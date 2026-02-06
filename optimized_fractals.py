# Imports functions from optimized_fractals.dll and nicely packages them
import math
import ctypes
import numpy as np
from PIL import Image

# Get all functions from optimized_fractals.dll
lib = ctypes.CDLL(r"C:\Users\aiden\Fractals\optimized_fractals_dll\build\Release\optimized_fractals.dll")

# Set up all the argtypes and restypes  
lib.get_last_error.restype = ctypes.c_char_p

# Class to pass coloring information into standard_fractal
class coloring(ctypes.Structure):
    _fields_ = [("coloring_option", ctypes.c_int32), ("palette", ctypes.c_int32 * 12), ("density", ctypes.c_int32)]


lib.standard_fractal.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, 
    ctypes.c_int,
    ctypes.POINTER(coloring),
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

lib.standard_julia.argtypes = [
    ctypes.c_float, ctypes.c_float, ctypes.c_float,
    ctypes.c_int, ctypes.c_int, ctypes.c_int, 
    ctypes.c_int,
    ctypes.c_int,
    ctypes.POINTER(ctypes.c_int),
    ctypes.POINTER(ctypes.c_ubyte),
    ctypes.c_float,
    ctypes.c_float,
    ctypes.POINTER(ctypes.c_float)
]

# Easier interface to make coloring class
def make_coloring(coloring_option, palette, density):
    return coloring(coloring_option, (ctypes.c_int32 * 12)(*palette), density)

def generate_standard_fractal(
        height, 
        width,
        scale,
        cx, 
        cy,
        max_iter,
        bailout_radius,
        coloring,
        coeffs,
        output_file
        ):

    coeffArray = ctypes.c_float * 52
    coeffs_c = coeffArray(*coeffs)

    buffer = np.zeros(width * height * 3, dtype=np.uint8)

    err = lib.standard_fractal(
        cx, 
        cy, 
        scale,
        width, 
        height, 
        max_iter,
        bailout_radius,
        ctypes.byref(coloring),
        buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
        coeffs_c
    )
    if err == -1:
        print(lib.get_last_error())

    img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
    img.save(output_file)


def generate_slow_fractal(
        height, 
        width,
        scale,
        cx, 
        cy,
        max_iter,
        coeffs,
        output_file
        ):

    coeffArray = ctypes.c_float * 52
    coeffs_c = coeffArray(*coeffs)

    buffer = np.zeros(width * height * 3, dtype=np.uint8)

    err = lib.slow_fractal(
        cx, 
        cy, 
        scale,
        width, 
        height, 
        max_iter,
        buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
        coeffs_c
    )
    if err == -1:
        print(lib.get_last_error())

    img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
    img.save(output_file)


def coolness_raw(
        height, 
        width,
        scale,
        cx, 
        cy,
        max_iter,
        coeffs
        ):

    coeffArray = ctypes.c_float * 52
    coeffs_c = coeffArray(*coeffs)

    surface_area_out = ctypes.c_int(1)
    size_out = ctypes.c_int(1)

    err = lib.coolness_raw(
        cx, 
        cy, 
        scale,
        width, 
        height, 
        max_iter,
        coeffs_c,
        ctypes.byref(surface_area_out),
        ctypes.byref(size_out)
    )
    if err == -1:
        print(lib.get_last_error())

    return (surface_area_out.value, size_out.value)


def surface_area(
        height, 
        width,
        scale,
        cx, 
        cy,
        coeffs
        ):
    
    # Calulate max_iter based on iteration_counts.ipynb, go a bit over just to be sure
    max_iter = int(math.sqrt(height*width)*0.7 + 10)

    coeffArray = ctypes.c_float * 52
    coeffs_c = coeffArray(*coeffs)

    surface_area_out = ctypes.c_int(1)

    err = lib.surface_area(
        cx, 
        cy, 
        scale,
        width, 
        height, 
        max_iter,
        coeffs_c,
        ctypes.byref(surface_area_out)
    )
    if err == -1:
        print(lib.get_last_error())

    return surface_area_out.value


def generate_standard_julia(
        height, 
        width,
        scale,
        cx, 
        cy,
        max_iter,
        bailout_radius,
        color_density,
        color_palette,        
        cr,
        ci,
        coeffs,
        output_file
        ):

    coeffArray = ctypes.c_float * 52
    coeffs_c = coeffArray(*coeffs)

    colorCoeffArray = ctypes.c_int * 12
    color_palette_c = colorCoeffArray(*color_palette)

    buffer = np.zeros(width * height * 3, dtype=np.uint8)

    err = lib.standard_julia(
        cx, 
        cy, 
        scale,
        width, 
        height, 
        max_iter,
        bailout_radius,
        color_density,
        color_palette_c,
        buffer.ctypes.data_as(ctypes.POINTER(ctypes.c_ubyte)),
        cr,
        ci,
        coeffs_c
    )
    if err == -1:
        print(lib.get_last_error())

    img = Image.frombuffer("RGB", (width, height), buffer, "raw", "RGB", 0, 1)
    img.save(output_file)
    
