from ctypes import *

so_file = "C:/Users/aiden/Fractals/my_functions.so"

my_functions = CDLL(so_file)

fileName = "test.png"

my_functions.greyscaleFractal(fileName.encode('utf-8'), 0, 1, 0, 1, 100, 100, 100)