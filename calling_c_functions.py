from ctypes import *

so_file = "C:/Users/aiden/Fractals/my_functions.so"

my_functions = CDLL(so_file)

print(my_functions.square(4))