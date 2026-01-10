from ctypes import *
import numpy as np
from numpy.ctypeslib import ndpointer
import duckdb
import pandas as pd


fractal_generation_functions = CDLL("C:/Users/aiden/Fractals/Release/fractal_generation.dll")



# Define input and output types for functions with array inputs
fractal_generation_functions.coolness.argtypes = [
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
    c_int
]
fractal_generation_functions.coolness.restype = c_float


fractal_generation_functions.formattedGreyscaleFractal.argtypes = [
    c_char_p,
    ndpointer(dtype=np.float32, flags="C_CONTIGUOUS"),
    c_int
]
fractal_generation_functions.formattedGreyscaleFractal.restype = int


# Create df to load data into
bufferDF = pd.DataFrame(None, columns= ["cReal", "cImag", "zbReal", "zbImag", "zsReal", "zsImag", "Coolness"])

# Connect to database
conn = duckdb.connect("FractalData.duckdb")

rounds = 0

parameters = [0.0] * 54

try:
    while True:

        rounds += 1

        print(rounds)

        for i in range(10):

            # Generate coefficients
            cReal = np.random.uniform(-2.0,2.0)
            cImag = np.random.uniform(-2.0,2.0)
            zbReal = np.random.uniform(-2.0,2.0)
            zbImag = np.random.uniform(-2.0,2.0)
            zsReal = np.random.uniform(-2.0,2.0)
            zsImag = np.random.uniform(-2.0,2.0)

            parameters[2] = cReal
            parameters[3] = cImag
            parameters[12] = zsReal
            parameters[13] = zsImag
            parameters[18] = zbReal
            parameters[19] = zbImag

            fractalResults = fractal_generation_functions.coolness(np.array(parameters, dtype=c_float), 1024)

            bufferDF.loc[i] = [cReal, cImag, zbReal, zbImag, zsReal, zsImag, fractalResults]

        conn.execute("INSERT INTO SimpleData\n SELECT * FROM bufferDF")
except KeyboardInterrupt:
    conn.close()

    

