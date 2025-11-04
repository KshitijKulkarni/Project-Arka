
import os
import sys
from PyQt6.QtGui import QGuiApplication
from PyQt6.QtQml import QQmlApplicationEngine


if __name__ == "__main__":
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    # Determine the path to the QML file dynamically
    script_dir = os.path.dirname(os.path.abspath(__file__))
    qml_file = os.path.join(script_dir, "QtGUI", "UntitledProject", "UntitledProjectContent", "Screen01.ui.qml")

    if not os.path.exists(qml_file):
        print(f"Error: QML file not found at {qml_file}", file=sys.stderr)
        sys.exit(1)

    engine.load(qml_file)

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec_())
