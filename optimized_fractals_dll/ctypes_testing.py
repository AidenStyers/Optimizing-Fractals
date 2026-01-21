# This file tests all functions able to be called from optimized_fractals.dll

import ctypes
import numpy as np
from PIL import Image


# Get all functions from optimized_fractals.dll
lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")


# Set up all the argtypes and restypes  
lib.get_last_error.restype = ctypes.c_char_p

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


lib.coolness_raw.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int,
    ctypes.POINTER(ctypes.c_float),
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_int)
]






# Call each function

width, height = 10000, 10000

# Set up mandelbrot coefficients for entry
coeffs = [  1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 
            0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
            0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
coeffArray = ctypes.c_float * 52
coeffs_c = coeffArray(*coeffs)


# Call standard_fractal

buffer = np.zeros(width * height * 3, dtype=np.uint8)
color_palette = [
    255, 0, 0,
    255, 255, 0, 
    0, 255, 255,
    0, 0, 255
]

colorCoeffArray = ctypes.c_int * 12
color_palette_c = colorCoeffArray(*color_palette)

err = lib.standard_fractal(
0.0, 0.0, 4.0 / width,
    width, height, 100,
    color_palette_c,
    buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
    coeffs_c
)
if err == -1:
    print(lib.get_last_error())

img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
img.save("standard_mandelbrot.png")



# Call slow_fractal
slow_fractal_buffer = np.zeros(width*height*3)
err = lib.slow_fractal(
    0.0, 0.0, 4.0 / width,
    width, height, 100,
    slow_fractal_buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
    coeffs_c
)
if err == -1:
    print(lib.get_last_error())

img = Image.frombuffer("RGB", (width, height), slow_fractal_buffer, "raw", "RGB", 0, 1)
img.save("slow_mandelbrot.png")



# Call coolness_raw

# Set up output variables
surface_area_out = ctypes.c_int(1)
size_out = ctypes.c_int(1)

coolness_raw_out = lib.coolness_raw(
    0.0, 0.0, 4.0 / width,
    width, height, 10000,
    coeffs_c,
    ctypes.byref(surface_area_out),
    ctypes.byref(size_out)
)

print(surface_area_out.value)
print(size_out.value)

print("Approx Mandelbrot set area: " + str((4.0/width)*(4.0/width)*size_out.value))
print("actual Mandelbrot set area is: 1.506591856")