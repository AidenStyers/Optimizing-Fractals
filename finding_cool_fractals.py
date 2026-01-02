from ctypes import *
import numpy as np
from numpy.ctypeslib import ndpointer


my_functions = CDLL("C:/Users/aiden/Fractals/Release/fractal_generation.dll")

# Define prototype using ndpointer for automatic type/flag checking
my_functions.coolness.argtypes = [
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS")
]
my_functions.coolness.restype = int

test = np.array([0, 0, 1, 0] + [0]*8 + [1,0] + [0]*40, dtype=c_float)

print(my_functions.coolness(test, 512))
