"""
Project ARKA - Ground Control Station (Non-OOP / Procedural Version)
Real-time telemetry display for Teensy 4.1 sensor data

This version uses the centralized gui_config module for ALL visual / timing /
serial / font / layout settings so the UI can be tweaked without editing this file.
"""

import customtkinter as ctk
import matplotlib.pyplot as plt
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import matplotlib as mpl
import serial
import serial.tools.list_ports
from threading import Thread
import time
from collections import deque

# centralized config (all colors/sizes/fonts/strings exposed here)
import gui_config as cfg

# apply matplotlib rc params from config
mpl.rcParams.update(cfg.MPL_RCPARAMS)

# Configure CustomTkinter appearance (appearance + theme remain outside cfg for now)
ctk.set_appearance_mode("light")
ctk.set_default_color_theme("blue")

# ========== GLOBAL VARIABLES ==========
serial_port = None
is_receiving = False
receive_thread = None

# data limits
max_data_points = cfg.MAX_DATA_POINTS
time_data = deque(maxlen=max_data_points)
ir_data = deque(maxlen=max_data_points)
req_flow_data = deque(maxlen=max_data_points)
actual_flow_data = deque(maxlen=max_data_points)
start_time = time.time()

# main window
root = ctk.CTk()
root.title(cfg.WINDOW_TITLE)
root.geometry(cfg.WINDOW_SIZE)
root.resizable(*cfg.WINDOW_RESIZABLE)

# ========== WIDGET PLACEHOLDERS ==========
port_dropdown = None
connect_btn = None
receive_btn = None
status_label = None
ir_value_label = None
req_flow_value_label = None
actual_flow_value_label = None
fig1 = None
ax1 = None
canvas1 = None
fig2 = None
ax2 = None
canvas2 = None

# ========== GUI CONSTRUCTION HELPERS ==========

def create_data_box(parent, title, color):
    """Create a standardized data display box using config values"""
    frame = ctk.CTkFrame(
        parent,
        fg_color=cfg.DATA_DISPLAY["frame_bg"],
        corner_radius=cfg.DATA_DISPLAY["corner_radius"],
    )
    title_font_size, title_weight = cfg.DATA_DISPLAY["title_font"]
    title_label = ctk.CTkLabel(
        frame,
        text=title,
        font=cfg.F(title_font_size, title_weight),
        text_color=color,
    )
    title_label.pack(**cfg.DATA_DISPLAY.get("title_pad", {"pady": (15, 5)}))
    return frame

def configure_graph(ax, xlabel, ylabel, color):
    """Apply configured styling to a matplotlib axis"""
    ax.set_facecolor(cfg.GRAPH_FRAME["axis_face"])
    ax.set_xlabel(xlabel, color=cfg.GRAPH_FRAME["axis_text_color"], fontsize=cfg.FONT_SIZES["tick"], fontfamily=cfg.FONT_FAMILY)
    ax.set_ylabel(ylabel, color=cfg.GRAPH_FRAME["axis_text_color"], fontsize=cfg.FONT_SIZES["tick"], fontfamily=cfg.FONT_FAMILY)
    ax.tick_params(colors=cfg.GRAPH_FRAME["axis_text_color"], labelsize=cfg.PLOT["tick_labelsize"])
    ax.grid(True, alpha=cfg.PLOT["grid_alpha"], color=cfg.GRAPH_FRAME["grid_color"], linestyle=cfg.PLOT["grid_linestyle"], linewidth=cfg.PLOT["grid_linewidth"])
    for spine in ax.spines.values():
        spine.set_edgecolor(cfg.GRAPH_FRAME["grid_color"])
        spine.set_linewidth(cfg.PLOT["grid_linewidth"])

# ========== SERIAL / PORT HELPERS ==========

def get_available_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports] if ports else [cfg.NO_PORTS_TEXT]

def refresh_ports():
    ports = get_available_ports()
    port_dropdown.configure(values=ports)
    if ports and ports[0] != cfg.NO_PORTS_TEXT:
        port_dropdown.set(ports[0])
    else:
        port_dropdown.set(cfg.NO_PORTS_TEXT)

def toggle_connection():
    global serial_port
    if serial_port and serial_port.is_open:
        disconnect()
    else:
        connect()

def connect():
    global serial_port
    port = port_dropdown.get()
    if port == cfg.SELECT_PORT_TEXT or port == cfg.NO_PORTS_TEXT:
        show_error("Please select a valid COM port")
        return
    try:
        serial_port = serial.Serial(port=port, baudrate=cfg.SERIAL_BAUDRATE, timeout=cfg.SERIAL_TIMEOUT)
        # update button / status using config values
        connect_btn.configure(text=cfg.CONTROL_PANEL["connect_button"]["text_disconnect"],
                              fg_color=cfg.CONTROL_PANEL["connect_button"]["fg_color_disconnect"],
                              hover_color=cfg.CONTROL_PANEL["connect_button"]["hover_color_disconnect"])
        receive_btn.configure(state="normal")
        status_label.configure(text=cfg.STATUS_TEXTS["connected"], text_color=cfg.TEXT_CONNECTED)
        port_dropdown.configure(state="disabled")
    except Exception as e:
        show_error(f"Connection failed: {str(e)}")

def disconnect():
    global serial_port, is_receiving
    if is_receiving:
        stop_receiving()
    if serial_port and serial_port.is_open:
        serial_port.close()
    connect_btn.configure(text=cfg.CONTROL_PANEL["connect_button"]["text_connect"],
                          fg_color=cfg.CONTROL_PANEL["connect_button"]["fg_color_connect"],
                          hover_color=cfg.CONTROL_PANEL["connect_button"]["hover_color_connect"])
    receive_btn.configure(state="disabled")
    status_label.configure(text=cfg.STATUS_TEXTS["disconnected"], text_color=cfg.TEXT_ERROR)
    port_dropdown.configure(state="normal")

def toggle_receive():
    global is_receiving
    if is_receiving:
        stop_receiving()
    else:
        start_receiving()

def start_receiving():
    global is_receiving, receive_thread, start_time
    is_receiving = True
    receive_btn.configure(text=cfg.CONTROL_PANEL["receive_button"]["text_stop"],
                          fg_color=cfg.CONTROL_PANEL["receive_button"]["fg_color_stop"],
                          hover_color=cfg.CONTROL_PANEL["receive_button"]["hover_color_stop"])
    status_label.configure(text=cfg.STATUS_TEXTS["receiving"], text_color=cfg.TEXT_RECEIVING)
    start_time = time.time()
    receive_thread = Thread(target=receive_data_loop, daemon=True)
    receive_thread.start()

def stop_receiving():
    global is_receiving
    is_receiving = False
    receive_btn.configure(text=cfg.CONTROL_PANEL["receive_button"]["text_receive"],
                          fg_color=cfg.CONTROL_PANEL["receive_button"]["fg_color_receive"],
                          hover_color=cfg.CONTROL_PANEL["receive_button"]["hover_color_receive"])
    status_label.configure(text=cfg.STATUS_TEXTS["connected"], text_color=cfg.TEXT_CONNECTED)

def receive_data_loop():
    """Background thread writing request bytes and reading/parsing responses."""
    while is_receiving:
        try:
            if serial_port and serial_port.is_open:
                # use configured request bytes and timing
                serial_port.write(cfg.SERIAL_REQUEST_BYTES)
                response = parse_response()
                if response:
                    ir_value, req_flow, actual_flow = response
                    root.after(0, update_data, ir_value, req_flow, actual_flow)
            time.sleep(cfg.RECEIVE_LOOP_INTERVAL)
        except Exception as e:
            print(f"Error in receive loop: {e}")
            root.after(0, show_error, f"Receive error: {str(e)}")
            break

def parse_response():
    """Parse device response using configured protocol bytes."""
    try:
        start_byte = serial_port.read(1)
        if not start_byte or start_byte[0] != cfg.SERIAL_EXPECTED_START:
            return None
        cmd_byte = serial_port.read(1)
        if not cmd_byte or cmd_byte[0] != cfg.SERIAL_EXPECTED_CMD:
            return None
        data_bytes = bytearray()
        while True:
            byte = serial_port.read(1)
            if not byte:
                return None
            if byte[0] == cfg.SERIAL_EXPECTED_END:
                break
            data_bytes.extend(byte)
        data_str = data_bytes.decode('ascii').strip()
        values = data_str.split(',')
        if len(values) == 3:
            ir_value = float(values[0])
            req_flow = float(values[1])
            actual_flow = float(values[2])
            return (ir_value, req_flow, actual_flow)
    except Exception as e:
        print(f"Parse error: {e}")
    return None

# ========== DATA / GRAPH UPDATE ==========
def update_data(ir_value, req_flow, actual_flow):
    current_time = time.time() - start_time
    time_data.append(current_time)
    ir_data.append(ir_value)
    req_flow_data.append(req_flow)
    actual_flow_data.append(actual_flow)
    # update labels (fonts/colors controlled by config when created)
    ir_value_label.configure(text=f"{ir_value:.0f}")
    req_flow_value_label.configure(text=f"{req_flow:.2f}")
    actual_flow_value_label.configure(text=f"{actual_flow:.2f}")

def update_graphs():
    """Refresh matplotlib plots using configured styling and schedule next refresh."""
    if len(time_data) > 0:
        # first graph (IR / RPM)
        ax1.clear()
        configure_graph(ax1, "Time (s)", "IR Value", cfg.COLOR_RPM)
        ax1.plot(list(time_data), list(ir_data), color=cfg.COLOR_RPM, linewidth=cfg.PLOT["linewidth"], label='IR Sensor')
        ax1.legend(facecolor=cfg.GRAPH_FRAME["legend_face"], edgecolor=cfg.GRAPH_FRAME["grid_color"],
                   labelcolor=cfg.GRAPH_FRAME["axis_text_color"], fontsize=cfg.PLOT["legend_fontsize"])
        canvas1.draw()
        # second graph (flow)
        ax2.clear()
        configure_graph(ax2, "Time (s)", "Flow Value", None)
        ax2.plot(list(time_data), list(req_flow_data), color=cfg.COLOR_REQ_FLOW, linewidth=cfg.PLOT["linewidth"], label='Required Flow')
        ax2.plot(list(time_data), list(actual_flow_data), color=cfg.COLOR_ACTUAL_FLOW, linewidth=cfg.PLOT["linewidth"], label='Actual Flow')
        ax2.legend(facecolor=cfg.GRAPH_FRAME["legend_face"], edgecolor=cfg.GRAPH_FRAME["grid_color"],
                   labelcolor=cfg.GRAPH_FRAME["axis_text_color"], fontsize=cfg.PLOT["legend_fontsize"])
        canvas2.draw()
    root.after(cfg.GRAPH_REFRESH_MS, update_graphs)

# ========== ERROR / WINDOW HANDLERS ==========
def show_error(message):
    error_window = ctk.CTkToplevel(root)
    error_window.title("Error")
    error_window.geometry(cfg.ERROR_WINDOW_SIZE)
    error_window.transient(root)
    error_window.grab_set()
    label = ctk.CTkLabel(
        error_window,
        text=message,
        font=cfg.F(cfg.FONT_SIZES["small"], cfg.FONT_WEIGHTS["normal"]),
        wraplength=350
    )
    label.pack(pady=20)
    ok_btn = ctk.CTkButton(
        error_window,
        text="OK",
        command=error_window.destroy,
        width=100,
        font=cfg.F(cfg.FONT_SIZES["small"], cfg.FONT_WEIGHTS["normal"])
    )
    ok_btn.pack(pady=(0, 20))

def on_closing():
    global is_receiving, serial_port
    if is_receiving:
        stop_receiving()
    if serial_port and serial_port.is_open:
        serial_port.close()
    root.destroy()

# ========== GUI LAYOUT CREATION ==========

main_frame = ctk.CTkFrame(root, fg_color=cfg.TRANSPARENT)
main_frame.pack(fill="both", expand=True, **cfg.MAIN_PADDING)

# HEADER
header_frame = ctk.CTkFrame(main_frame, fg_color=cfg.HEADER["fg_color"], corner_radius=cfg.HEADER["corner_radius"])
header_frame.pack(fill="x", pady=cfg.PADDING_MEDIUM)
title_font_size, title_weight = cfg.HEADER["title_font"]
title_label = ctk.CTkLabel(header_frame, text="PROJECT ARKA", font=cfg.F(title_font_size, title_weight), text_color=cfg.HEADER["title_color"])
title_label.pack(**cfg.HEADER.get("title_pad", {"pady": (20, 5)}))
subtitle_font_size, subtitle_weight = cfg.HEADER["subtitle_font"]
subtitle_label = ctk.CTkLabel(header_frame, text="Ground Control Station", font=cfg.F(subtitle_font_size, subtitle_weight), text_color=cfg.HEADER["subtitle_color"])
subtitle_label.pack(**cfg.HEADER.get("subtitle_pad", {"pady": (0, 20)}))

# CONTROL PANEL
control_frame = ctk.CTkFrame(main_frame, fg_color=cfg.CONTROL_PANEL["fg_color"], corner_radius=cfg.CONTROL_PANEL["corner_radius"])
control_frame.pack(fill="x", pady=cfg.PADDING_MEDIUM)
control_inner = ctk.CTkFrame(control_frame, fg_color=cfg.CONTROL_PANEL["inner_fg_color"])
control_inner.pack(fill="x", padx=cfg.MAIN_PADDING["padx"], pady=cfg.PADDING_MEDIUM)
port_label = ctk.CTkLabel(control_inner, text="COM Port:", font=cfg.F(*cfg.CONTROL_PANEL["port_label_font"]))
port_label.grid(row=0, column=0, padx=(0, cfg.PADDING_MEDIUM[0]), sticky="w")

port_dropdown = ctk.CTkComboBox(
    control_inner,
    values=get_available_ports(),
    width=cfg.CONTROL_PANEL["port_dropdown_width"],
    font=cfg.F(*cfg.CONTROL_PANEL["port_dropdown_font"])
)
port_dropdown.grid(row=0, column=1, padx=(0, cfg.PADDING_MEDIUM[0]))
port_dropdown.set(cfg.SELECT_PORT_TEXT)

# Refresh button (use config)
refresh_cfg = cfg.CONTROL_PANEL["refresh_button"]
refresh_btn = ctk.CTkButton(
    control_inner,
    text=refresh_cfg["text"],
    width=refresh_cfg["width"],
    command=refresh_ports,
    fg_color=refresh_cfg["fg_color"],
    hover_color=refresh_cfg["hover_color"],
    font=cfg.F(*refresh_cfg["font"])
)
refresh_btn.grid(row=0, column=2, padx=(0, cfg.PADDING_MEDIUM[0]))

# Connect button (use config)
connect_cfg = cfg.CONTROL_PANEL["connect_button"]
connect_btn = ctk.CTkButton(
    control_inner,
    text=connect_cfg["text_connect"],
    width=connect_cfg["width"],
    command=toggle_connection,
    fg_color=connect_cfg["fg_color_connect"],
    hover_color=connect_cfg["hover_color_connect"],
    font=cfg.F(*connect_cfg["font"])
)
connect_btn.grid(row=0, column=3, padx=(0, cfg.PADDING_MEDIUM[0]))

# Receive button
receive_cfg = cfg.CONTROL_PANEL["receive_button"]
receive_btn = ctk.CTkButton(
    control_inner,
    text=receive_cfg["text_receive"],
    width=receive_cfg["width"],
    command=toggle_receive,
    state="disabled",
    fg_color=receive_cfg["fg_color_receive"],
    hover_color=receive_cfg["hover_color_receive"],
    font=cfg.F(*receive_cfg["font"])
)
receive_btn.grid(row=0, column=4)

status_label = ctk.CTkLabel(
    control_inner,
    text=cfg.STATUS_TEXTS["disconnected"],
    font=cfg.F(*cfg.CONTROL_PANEL["status_label_font"]),
    text_color=cfg.TEXT_ERROR
)
status_label.grid(row=0, column=5, padx=(cfg.PADDING_MEDIUM[0], 0))

# DATA DISPLAY
data_frame = ctk.CTkFrame(main_frame, fg_color=cfg.FRAME_BG, corner_radius=cfg.FRAME_CORNER_RADIUS)
data_frame.pack(fill="x", pady=cfg.PADDING_MEDIUM)
data_inner = ctk.CTkFrame(data_frame, fg_color=cfg.TRANSPARENT)
data_inner.pack(fill="x", padx=cfg.MAIN_PADDING["padx"], pady=cfg.PADDING_MEDIUM)
data_inner.grid_columnconfigure(0, weight=1)
data_inner.grid_columnconfigure(1, weight=1)
data_inner.grid_columnconfigure(2, weight=1)

ir_frame = create_data_box(data_inner, "RPM", cfg.COLOR_RPM)
ir_frame.grid(row=0, column=0, padx=cfg.PADDING_MEDIUM[0], sticky="ew")
ir_value_label = ctk.CTkLabel(ir_frame, text="0", font=cfg.F(*cfg.DATA_DISPLAY["value_font_large"]), text_color=cfg.COLOR_RPM)
ir_value_label.pack(**cfg.DATA_DISPLAY.get("value_pad", {"pady": (10, 15)}))

req_flow_frame = create_data_box(data_inner, "REQUIRED FLOW RATE (L/min)", cfg.COLOR_REQ_FLOW)
req_flow_frame.grid(row=0, column=1, padx=cfg.PADDING_MEDIUM[0], sticky="ew")
req_flow_value_label = ctk.CTkLabel(req_flow_frame, text="0.00", font=cfg.F(*cfg.DATA_DISPLAY["value_font_large"]), text_color=cfg.COLOR_REQ_FLOW)
req_flow_value_label.pack(**cfg.DATA_DISPLAY.get("value_pad", {"pady": (10, 15)}))

actual_flow_frame = create_data_box(data_inner, "ACTUAL FLOW RATE (L/min)", cfg.COLOR_ACTUAL_FLOW)
actual_flow_frame.grid(row=0, column=2, padx=cfg.PADDING_MEDIUM[0], sticky="ew")
actual_flow_value_label = ctk.CTkLabel(actual_flow_frame, text="0.00", font=cfg.F(*cfg.DATA_DISPLAY["value_font_large"]), text_color=cfg.COLOR_ACTUAL_FLOW)
actual_flow_value_label.pack(**cfg.DATA_DISPLAY.get("value_pad", {"pady": (10, 15)}))

# GRAPHS
graphs_frame = ctk.CTkFrame(main_frame, fg_color=cfg.TRANSPARENT)
graphs_frame.pack(fill="both", expand=True)
graphs_frame.grid_columnconfigure(0, weight=1)
graphs_frame.grid_columnconfigure(1, weight=1)
graphs_frame.grid_rowconfigure(0, weight=1)

ir_graph_frame = ctk.CTkFrame(graphs_frame, fg_color=cfg.GRAPH_FRAME["fg_color"], corner_radius=cfg.GRAPH_FRAME["corner_radius"])
ir_graph_frame.grid(row=0, column=0, padx=(0, cfg.PADDING_MEDIUM[0]), sticky="nsew")
gt_size, gt_weight = cfg.GRAPH_FRAME["graph_title_font"]
graph1_title = ctk.CTkLabel(ir_graph_frame, text="RPM vs TIME", font=cfg.F(gt_size, gt_weight), text_color=cfg.COLOR_RPM)
graph1_title.pack(**cfg.GRAPH_FRAME.get("graph_title_pad", {"pady": (15, 5)}))

fig1 = Figure(figsize=cfg.GRAPH_FRAME["figure_size"], facecolor=cfg.GRAPH_FRAME["figure_facecolor"])
ax1 = fig1.add_subplot(111)
configure_graph(ax1, "Time (s)", "RPM", cfg.COLOR_RPM)
canvas1 = FigureCanvasTkAgg(fig1, ir_graph_frame)
canvas1.get_tk_widget().pack(fill="both", expand=True, padx=cfg.MAIN_PADDING["padx"] // 2, pady=(0, cfg.PADDING_MEDIUM[1]))

flow_graph_frame = ctk.CTkFrame(graphs_frame, fg_color=cfg.GRAPH_FRAME["fg_color"], corner_radius=cfg.GRAPH_FRAME["corner_radius"])
flow_graph_frame.grid(row=0, column=1, padx=(cfg.PADDING_MEDIUM[0], 0), sticky="nsew")
graph2_title = ctk.CTkLabel(flow_graph_frame, text="FLOW RATE vs TIME", font=cfg.F(gt_size, gt_weight), text_color=cfg.COLOR_REQ_FLOW)
graph2_title.pack(**cfg.GRAPH_FRAME.get("graph_title_pad", {"pady": (15, 5)}))

fig2 = Figure(figsize=cfg.GRAPH_FRAME["figure_size"], facecolor=cfg.GRAPH_FRAME["figure_facecolor"])
ax2 = fig2.add_subplot(111)
configure_graph(ax2, "Time (s)", "Flow Rate Value (L/min)", None)
canvas2 = FigureCanvasTkAgg(fig2, flow_graph_frame)
canvas2.get_tk_widget().pack(fill="both", expand=True, padx=cfg.MAIN_PADDING["padx"] // 2, pady=(0, cfg.PADDING_MEDIUM[1]))

# ========== MAINLOOP START ==========
root.protocol("WM_DELETE_WINDOW", on_closing)
root.after(cfg.GRAPH_REFRESH_MS, update_graphs)
root.mainloop()
