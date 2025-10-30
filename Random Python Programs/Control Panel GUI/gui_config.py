"""
gui_config.py

Comprehensive, well-labeled centralized configuration for Project ARKA GUI.

Purpose:
- Expose ALL hardcoded values used by the GUI (colors, fonts, sizes, paddings,
  widget-specific parameters, matplotlib defaults, serial/protocol values, timing).
- Make it trivial to customize appearance & behaviour of every widget used by GUI.py.
- Provide a small helper F(...) that returns a CustomTkinter font (keeps call-sites unchanged).

Notes:
- This module imports customtkinter solely to produce CTkFont objects via F(...).
  If you need to use gui_config in environments without customtkinter, replace/guard
  the F(...) helper in your test environment or create a thin compatibility shim.
"""

from typing import Dict, Tuple
import customtkinter as ctk

# ---------------------------------------------------------------------------
# FONT / TYPOGRAPHY
# ---------------------------------------------------------------------------

# Primary font family used throughout the app. Set to "Infynyte" as requested.
FONT_FAMILY: str = "Infynyte Body"

# Numeric sizes used by widgets. Adjust here to scale the entire UI.
FONT_SIZES: Dict[str, int] = {
    "title": 42,           # main header title
    "subtitle": 24,        # header subtitle
    "label": 14,           # generic form/field labels
    "small": 13,           # small text (status, help)
    "graph_title": 16,     # titles above graphs
    "value_large": 36,     # large numeric readouts (RPM, flow)
    "legend": 9,           # legend text size on matplotlib
    "tick": 9,             # graph tick label size (matplotlib)
}

# Font weight names used in CTkFont calls
FONT_WEIGHTS: Dict[str, str] = {
    "normal": "normal",
    "bold": "bold",
    "heavy": "bold",   # alias if you want a heavier look
}

def F(size: int = None, weight: str = "normal"):
    """
    Return a CustomTkinter CTkFont configured to the standardized family/size/weight.
    Call-sites in GUI.py expect this to return a CTkFont.
    Example: font=cfg.F(cfg.FONT_SIZES['label'], cfg.FONT_WEIGHTS['bold'])
    """
    if size is None:
        size = FONT_SIZES["label"]
    return ctk.CTkFont(family=FONT_FAMILY, size=size, weight=weight)

# ---------------------------------------------------------------------------
# WINDOW / APP
# ---------------------------------------------------------------------------

WINDOW_TITLE: str = "Project ARKA - Ground Control"
WINDOW_SIZE: str = "1920x1080"     # geometry string
WINDOW_RESIZABLE: Tuple[bool, bool] = (True, True)
MAIN_PADDING: Dict[str, int] = {"padx": 20, "pady": 20}

# ---------------------------------------------------------------------------
# LAYOUT / SPACING / PACK / GRID defaults
# ---------------------------------------------------------------------------

# Generic frame corner radius and internal paddings
FRAME_CORNER_RADIUS: int = 15
DATA_BOX_CORNER_RADIUS: int = 10

# Standard widget paddings used across layout creation (pack/grid)
PADDING_SMALL: Tuple[int, int] = (5, 5)
PADDING_MEDIUM: Tuple[int, int] = (10, 10)
PADDING_LARGE: Tuple[int, int] = (15, 15)

# ---------------------------------------------------------------------------
# COLORS (CENTRALIZED)
# ---------------------------------------------------------------------------

# Backgrounds
TRANSPARENT: str = "transparent"
FRAME_BG: str = "#c8c8ff"      # primary frame background
DATA_BOX_BG: str = "#3d3d8d"   # inside data/display boxes
FIG_FACE: str = FRAME_BG       # matplotlib fig facecolor
AXIS_FACE: str = DATA_BOX_BG   # matplotlib axis facecolor

# Text / axis / legend
AXIS_TEXT: str = "#a8a8a8"
GRID_COLOR: str = "#555555"
LEGEND_FACE: str = "#1a1a2e"

# Semantic status colors
TEXT_ERROR: str = "#ff4444"
TEXT_CONNECTED: str = "#00ff88"
TEXT_RECEIVING: str = "#00d9ff"

# Header / title colors
TITLE_COLOR: str = "#31245f"
SUBTITLE_COLOR: str = "#432f8d"

# Value display colors
COLOR_RPM: str = "#ff6b35"
COLOR_REQ_FLOW: str = "#00d9ff"
COLOR_ACTUAL_FLOW: str = "#00ff88"

# ---------------------------------------------------------------------------
# BUTTON / CONTROL COLORS & DEFAULTS
# ---------------------------------------------------------------------------

# Connect / Disconnect
BTN_CONNECT: str = "#00a859"
BTN_CONNECT_HOVER: str = "#00c96b"
BTN_DISCONNECT: str = "#ff4444"
BTN_DISCONNECT_HOVER: str = "#ff6666"

# Receive / Stop
BTN_RECEIVE: str = "#0077b6"
BTN_RECEIVE_HOVER: str = "#0096d6"
BTN_STOP: str = "#ff6b35"
BTN_STOP_HOVER: str = "#ff8b55"

# Refresh / other
BTN_REFRESH: str = "#2d4263"
BTN_REFRESH_HOVER: str = "#3d5273"

# Generic CTkButton defaults that GUI can apply when constructing new buttons
DEFAULT_BTN_WIDTH: int = 120
DEFAULT_BTN_HEIGHT: int = 32
DEFAULT_BTN_CORNER_RADIUS: int = 8

# ---------------------------------------------------------------------------
# WIDGET-SPECIFIC VISUALS (expose everything the GUI uses)
# ---------------------------------------------------------------------------

# Header
HEADER = {
    "fg_color": FRAME_BG,
    "corner_radius": FRAME_CORNER_RADIUS,
    "title_font": (FONT_SIZES["title"], FONT_WEIGHTS["bold"]),
    "subtitle_font": (FONT_SIZES["subtitle"], FONT_WEIGHTS["normal"]),
    "title_color": TITLE_COLOR,
    "subtitle_color": SUBTITLE_COLOR,
    "title_pad": {"pady": (20, 20)},
    "subtitle_pad": {"pady": (0, 20)},
}

# Control panel / port chooser
CONTROL_PANEL = {
    "fg_color": FRAME_BG,
    "corner_radius": FRAME_CORNER_RADIUS,
    "inner_fg_color": TRANSPARENT,
    "port_label_font": (FONT_SIZES["label"], FONT_WEIGHTS["normal"]),
    "port_dropdown_width": 200,
    "port_dropdown_font": (FONT_SIZES["small"], FONT_WEIGHTS["normal"]),
    "refresh_button": {
        "text": "🔄 Refresh",
        "width": 100,
        "fg_color": BTN_REFRESH,
        "hover_color": BTN_REFRESH_HOVER,
        "font": (FONT_SIZES["small"], FONT_WEIGHTS["normal"]),
    },
    "connect_button": {
        "text_connect": "Connect",
        "text_disconnect": "Disconnect",
        "width": 120,
        "fg_color_connect": BTN_CONNECT,
        "hover_color_connect": BTN_CONNECT_HOVER,
        "fg_color_disconnect": BTN_DISCONNECT,
        "hover_color_disconnect": BTN_DISCONNECT_HOVER,
        "font": (FONT_SIZES["label"], FONT_WEIGHTS["bold"]),
    },
    "receive_button": {
        "text_receive": "▶ Receive Data",
        "text_stop": "⏸ Stop",
        "width": 150,
        "fg_color_receive": BTN_RECEIVE,
        "hover_color_receive": BTN_RECEIVE_HOVER,
        "fg_color_stop": BTN_STOP,
        "hover_color_stop": BTN_STOP_HOVER,
        "font": (FONT_SIZES["label"], FONT_WEIGHTS["bold"]),
    },
    "status_label_font": (FONT_SIZES["small"], FONT_WEIGHTS["normal"]),
}

# Data display boxes
DATA_DISPLAY = {
    "frame_bg": DATA_BOX_BG,
    "corner_radius": DATA_BOX_CORNER_RADIUS,
    "title_font": (FONT_SIZES["label"], FONT_WEIGHTS["bold"]),
    "value_font_large": (FONT_SIZES["value_large"], FONT_WEIGHTS["bold"]),
    "value_pad": {"pady": (10, 15)},
}

# Graph frames / titles
GRAPH_FRAME = {
    "fg_color": FRAME_BG,
    "corner_radius": FRAME_CORNER_RADIUS,
    "graph_title_font": (FONT_SIZES["graph_title"], FONT_WEIGHTS["bold"]),
    "graph_title_pad": {"pady": (15, 5)},
    "figure_size": (6, 4),   # in inches supplied to matplotlib Figure(figsize=...)
    "figure_facecolor": FIG_FACE,
    "axis_face": AXIS_FACE,
    "axis_text_color": AXIS_TEXT,
    "legend_face": LEGEND_FACE,
    "grid_color": GRID_COLOR,
    "plot_linewidth": 2,
    "tick_labelsize": FONT_SIZES["tick"],
}

# ---------------------------------------------------------------------------
# MATPLOTLIB / PLOTTING DEFAULTS
# ---------------------------------------------------------------------------

# High-level rcParams you might want to expose
MPL_RCPARAMS = {
    "font.family": FONT_FAMILY,
    "axes.facecolor": FIG_FACE,
    "figure.facecolor": FIG_FACE,
    "legend.facecolor": LEGEND_FACE,
    "grid.color": GRID_COLOR,
    "axes.labelcolor": AXIS_TEXT,
    "xtick.color": AXIS_TEXT,
    "ytick.color": AXIS_TEXT,
}

# Plot behaviour
PLOT = {
    "grid_alpha": 0.2,
    "grid_linestyle": "--",
    "grid_linewidth": 0.5,
    "linewidth": 2,
    "tick_labelsize": 9,
    "legend_fontsize": FONT_SIZES["legend"],
}

# ---------------------------------------------------------------------------
# SERIAL / PROTOCOL / TIMING
# ---------------------------------------------------------------------------

MAX_DATA_POINTS: int = 100
SERIAL_BAUDRATE: int = 115200
SERIAL_TIMEOUT: float = 0.1

# Bytes used to request data from the device (customize if protocol changes)
SERIAL_REQUEST_BYTES: bytes = bytes([0x02, 0x11, 0x03])
SERIAL_EXPECTED_START: int = 0x02
SERIAL_EXPECTED_CMD: int = 0x81
SERIAL_EXPECTED_END: int = 0x03

# Receive loop timing (seconds)
RECEIVE_LOOP_INTERVAL: float = 0.1
GRAPH_REFRESH_MS: int = 100   # GUI.after(...) interval for graph updates

# ---------------------------------------------------------------------------
# TEXT / STRINGS / LABELS
# ---------------------------------------------------------------------------

SELECT_PORT_TEXT: str = "Select Port"
NO_PORTS_TEXT: str = "No ports found"
ERROR_WINDOW_SIZE: str = "400x150"

# Standardized status label texts (used by GUI)
STATUS_TEXTS = {
    "disconnected": "● Disconnected",
    "connected": "● Connected",
    "receiving": "● Receiving Data",
}

# ---------------------------------------------------------------------------
# UTILITY / DEFAULT VALUES
# ---------------------------------------------------------------------------

DEFAULT_MAX_DATA_POINTS: int = 100

# ---------------------------------------------------------------------------
# EXPORTS
# ---------------------------------------------------------------------------

__all__ = [
    # fonts
    "FONT_FAMILY", "FONT_SIZES", "FONT_WEIGHTS", "F",
    # window
    "WINDOW_TITLE", "WINDOW_SIZE", "WINDOW_RESIZABLE", "MAIN_PADDING",
    # layout
    "FRAME_CORNER_RADIUS", "DATA_BOX_CORNER_RADIUS",
    "PADDING_SMALL", "PADDING_MEDIUM", "PADDING_LARGE",
    # colors
    "TRANSPARENT", "FRAME_BG", "DATA_BOX_BG", "FIG_FACE", "AXIS_FACE",
    "AXIS_TEXT", "GRID_COLOR", "LEGEND_FACE",
    "TEXT_ERROR", "TEXT_CONNECTED", "TEXT_RECEIVING",
    "TITLE_COLOR", "SUBTITLE_COLOR",
    # button colors
    "BTN_CONNECT", "BTN_CONNECT_HOVER", "BTN_DISCONNECT", "BTN_DISCONNECT_HOVER",
    "BTN_RECEIVE", "BTN_RECEIVE_HOVER", "BTN_STOP", "BTN_STOP_HOVER",
    "BTN_REFRESH", "BTN_REFRESH_HOVER",
    "DEFAULT_BTN_WIDTH", "DEFAULT_BTN_HEIGHT", "DEFAULT_BTN_CORNER_RADIUS",
    # displays
    "COLOR_RPM", "COLOR_REQ_FLOW", "COLOR_ACTUAL_FLOW",
    # widget groupings
    "HEADER", "CONTROL_PANEL", "DATA_DISPLAY", "GRAPH_FRAME",
    # matplotlib/plotting
    "MPL_RCPARAMS", "PLOT",
    # serial/protocol
    "MAX_DATA_POINTS", "SERIAL_BAUDRATE", "SERIAL_TIMEOUT",
    "SERIAL_REQUEST_BYTES", "SERIAL_EXPECTED_START", "SERIAL_EXPECTED_CMD", "SERIAL_EXPECTED_END",
    "RECEIVE_LOOP_INTERVAL", "GRAPH_REFRESH_MS",
    # strings
    "SELECT_PORT_TEXT", "NO_PORTS_TEXT", "ERROR_WINDOW_SIZE", "STATUS_TEXTS",
]