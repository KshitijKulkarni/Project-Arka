import sys
# import PyQt6.QtCore
from PySide6.QtGui import QGuiApplication
from PySide6.QtQml import QQmlApplicationEngine

from autogen.settings import setup_qt_environment
from connector import Backend

# Import here the Python files that define QML elements


def main():
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    setup_qt_environment(engine)

    # Expose the Python object to QML
    backend = Backend()
    engine.rootContext().setContextProperty("backend", backend)

    qml_file = "../UntitledProjectContent/App.qml"
    engine.load(qml_file)

    if not engine.rootObjects():
        sys.exit(-1)

    ex = app.exec()
    del engine
    return ex

5
if __name__ == "__main__":
    sys.exit(main())
