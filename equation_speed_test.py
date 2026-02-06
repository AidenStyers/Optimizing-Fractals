# Find speedup of optimized equations on actual fractal generation
import time
import optimized_fractals as of
import random

runs = 10000
resolution = 2048

coloring = of.make_coloring(1, [0]*12, 50) # Coloring is never actually used

# Generate long list of coeffs
coeff_list = [ [random.uniform(-2, 2) for _ in range(52)] for _ in range(runs)]

start_time = time.perf_counter()

for coeffs in coeff_list:

    of.generate_standard_fractal(
        resolution,
        resolution,
        4.0 / resolution,
        0.0,
        0.0,
        200,
        100000,
        coloring,
        coeffs,
        "images/random_filler.png"
    )

print( "Optimized time: " + str(time.perf_counter() - start_time))


start_time = time.perf_counter()

for coeffs in coeff_list:

    of.generate_slow_fractal(
        resolution,
        resolution,
        4.0 / resolution,
        0.0,
        0.0,
        200,
        coeffs,
        "images/random_filler.png"
    )

print( "Unoptimized time: " + str(time.perf_counter() - start_time))