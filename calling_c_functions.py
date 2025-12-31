from ctypes import *

so_file = "C:/Users/aiden/Fractals/my_functions.so"

my_functions = CDLL(so_file)

fileName = "test3.png"

my_functions.greyscaleFractal(fileName.encode('utf-8'), c_float(-2), c_float(2), c_float(-2), c_float(2), 1000, 1000, 50)