from PySide6.QtCore import QObject, Slot, Signal, QThread
import serial
import time

class SerialReader(QThread):
    data_received = Signal(str)

    def __init__(self, serial_port):
        super().__init__()
        self.serial_port = serial_port
        self._running = True

    def run(self):
        while self._running:
            if self.serial_port and self.serial_port.is_open:
                try:
                    if self.serial_port.in_waiting > 0:
                        data = self.serial_port.readline().decode().strip()
                        self.data_received.emit(data)
                except Exception as e:
                    print(f"Error reading serial: {e}")
            time.sleep(0.1)

    def stop(self):
        self._running = False
        self.wait()

class Backend(QObject):
    rpm_data_received = Signal(str)
    connection_successful = Signal()
    connection_error = Signal()

    def __init__(self):
        super().__init__()
        self.serial_port = None
        self.serial_reader_thread = None

    @Slot(str, str, result=str)
    def connect_button_clicked(self, port, baud_rate):
        print(f"Attempting to connect to {port} at {baud_rate} baud.")
        try:
            if self.serial_port and self.serial_port.is_open:
                self.serial_port.close()
            self.serial_port = serial.Serial(port, int(baud_rate), timeout=0.1)
            print(f"Successfully connected to {port}")

            if self.serial_reader_thread and self.serial_reader_thread.isRunning():
                self.serial_reader_thread.stop()

            self.serial_reader_thread = SerialReader(self.serial_port)
            self.serial_reader_thread.data_received.connect(self.rpm_data_received)
            self.serial_reader_thread.start()

            self.connection_successful.emit()
            return "Connected"
        except Exception as e:
            print(f"Failed to connect: {e}")
            self.connection_error.emit()
            return f"Failed to connect: {e}"

    @Slot(str, result=str)
    def small_radio_buttons_clicked(self, button_id):
        print(f"Small radio button {button_id} clicked!")
        return f"{button_id} handled by small group"

    @Slot(str, result=str)
    def medium_radio_buttons_clicked(self, button_id):
        print(f"Medium radio button {button_id} clicked!")
        return f"{button_id} handled by medium group"

    @Slot(str, result=str)
    def large_radio_buttons_clicked(self, button_id):
        print(f"Large radio button {button_id} clicked!")
        return f"{button_id} handled by large group"
