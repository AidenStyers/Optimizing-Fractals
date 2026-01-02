from ctypes import *

so_file = r"C:/Users/aiden/Fractals/fractal_generation.dll"

my_functions = CDLL(so_file)

fileName = "test3.png"

my_functions.greyscaleFractal(fileName.encode('utf-8'), c_float(-2), c_float(2), c_float(-2), c_float(2), 1000, 1000, 50)
