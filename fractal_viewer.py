import sys
import math
from PyQt6.QtWidgets import QApplication, QWidget
from PyQt6.QtGui import QPainter, QColor, QImage
from PyQt6.QtCore import Qt, QPointF, QTimer
import optimized_fractals as of
import numpy as np
import ctypes
from fractal_parameters import fractal_parameters

# Get all functions from optimized_fractals.dll
lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")

# Set up all the argtypes and restypes  
lib.get_last_error.restype = ctypes.c_char_p

lib.standard_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, 
    ctypes.c_int,
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.POINTER(ctypes.c_float)
]

lib.slow_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.POINTER(ctypes.c_float)
]

lib.coolness_raw.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_int)
]

lib.surface_area.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_int)
]



# --------------------------------------------------
# Viewer
# --------------------------------------------------
class FractalViewer(QWidget):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Fractal Explorer")
        self.resize(900, 700)

        # View state
        self.center_x = 0.0
        self.center_y = 0.0
        self.scale = 3.0 / self.width()  # world units per pixel

        # Interaction state
        self.last_mouse_pos = None
        self.pending_reload = False

        # Thresholds
        self.pan_epsilon = 1
        self.zoom_epsilon = 0.02

        # Debounce reloads
        self.reload_timer = QTimer(self)
        self.reload_timer.setSingleShot(True)
        self.reload_timer.timeout.connect(self._do_reload)

        self._do_reload()

    # --------------------------------------------------


    # --------------------------------------------------
    # YOU provide this
    # --------------------------------------------------
    def reload_fractal(self, view):
        """
        Called when the fractal needs to be regenerated.

        view = {
            "center_x": float,
            "center_y": float,
            "scale": float,
            "width": int,
            "height": int
        }
        """

        global fractal_parameters

        color_palette = [
            51, 51, 255,
            0, 153, 200,
            0, 230, 0,
            51, 51, 255
        ]

        of.generate_standard_fractal(
            view["height"],
            view["width"],
            view["scale"],
            view["center_x"],
            view["center_y"],
            100*math.floor(1 + math.log(1/view["scale"])),
            100,
            20,
            color_palette,
            fractal_parameters,
            "displayed_image.png"
        )

        self.image = QImage(
            "displayed_image.png"
        )




    def _schedule_reload(self):
        if not self.reload_timer.isActive():
            self.reload_timer.start(80)  # ms debounce

    def _do_reload(self):
        self.reload_fractal({
            "center_x": self.center_x,
            "center_y": self.center_y,
            "scale": self.scale,
            "width": self.width(),
            "height": self.height()
        })
        self.update()

    # --------------------------------------------------
    # Events
    # --------------------------------------------------

    def mousePressEvent(self, event):
        if event.button() == Qt.MouseButton.LeftButton:
            self.last_mouse_pos = event.position()

    def mouseMoveEvent(self, event):
        if self.last_mouse_pos is None:
            return

        delta = event.position() - self.last_mouse_pos
        self.last_mouse_pos = event.position()

        dx = -delta.x() * self.scale
        dy = -delta.y() * self.scale

        if abs(delta.x()) > self.pan_epsilon or abs(delta.y()) > self.pan_epsilon:
            self.center_x += dx
            self.center_y += dy
            self._schedule_reload()

    def mouseReleaseEvent(self, event):
        self.last_mouse_pos = None

    def wheelEvent(self, event):
        zoom_factor = math.exp(event.angleDelta().y() * 0.001)

        old_scale = self.scale
        self.scale *= zoom_factor

        if abs(self.scale / old_scale - 1.0) > self.zoom_epsilon:
            self._schedule_reload()

    def resizeEvent(self, event):
        self._schedule_reload()

    # --------------------------------------------------
    # Placeholder rendering
    # --------------------------------------------------

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.fillRect(self.rect(), QColor(10, 10, 10))
        
        if self.image:
            painter.drawImage(self.rect(), self.image)

        painter.setPen(QColor(200, 200, 200))
        painter.drawText(10, 20, f"Center: ({self.center_x:.6f}, {self.center_y:.6f})")
        painter.drawText(10, 40, f"Scale: {self.scale:.6e}")


# --------------------------------------------------
if __name__ == "__main__":
    app = QApplication(sys.argv)
    w = FractalViewer()
    w.show()
    sys.exit(app.exec())
