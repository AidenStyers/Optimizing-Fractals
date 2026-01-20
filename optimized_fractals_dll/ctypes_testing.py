import ctypes
import numpy as np
from numpy.ctypeslib import ndpointer
from PIL import Image

lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")

lib.standard_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.POINTER(ctypes.c_float)
]

lib.get_last_error.restype = ctypes.c_char_p

width, height = 1920, 1080
buffer = np.zeros(width * height * 3, dtype=np.uint8)

coeffs = [  1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
coeffArray = ctypes.c_float * 52
coeffs_c = coeffArray(*coeffs)

err = lib.standard_fractal(
    -0.75, 0.0, 0.002,
    width, height, 1000, 1,
    buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
    coeffs_c
)
if err == -1:
    print(lib.get_last_error())

img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
img.save("mandelbrot.png")