import ctypes
import numpy as np
from numpy.ctypeslib import ndpointer
from PIL import Image

lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")

lib.standard_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, 
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

lib.get_last_error.restype = ctypes.c_char_p

width, height = 1920, 1080
buffer = np.zeros(width * height * 3, dtype=np.uint8)

# Set up coefficients for entry
coeffs = [  1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
coeffArray = ctypes.c_float * 52
coeffs_c = coeffArray(*coeffs)

# Set up color palette for entry
color_palette = [
    0, 0, 204,
    255, 255, 255, 
    255, 128, 0,
    64, 64, 64
]
colorCoeffArray = ctypes.c_int * 12
color_palette_c = colorCoeffArray(*color_palette)

err = lib.standard_fractal(
    -0.75, 0.0, 0.002,
    width, height, 1000,
    color_palette_c,
    buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
    coeffs_c
)
if err == -1:
    print(lib.get_last_error())

img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
img.save("mandelbrot.png")