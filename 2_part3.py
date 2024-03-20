import numpy as np
import time

def timer(func, *args, **kwargs):
    start_time = time.time()
    result = func(*args, **kwargs)
    end_time = time.time()
    duration = (end_time - start_time) * 1e6  # Convert to microseconds
    print(f"{func.__name__} took {duration:.2f} microseconds")
    return result

def vector_operations(n, num_iterations):
    # Initialize random number generator
    rng = np.random.default_rng()

    # Create NumPy arrays filled with random doubles
    b = rng.uniform(-1.0, 1.0, size=n)
    c = rng.uniform(-1.0, 1.0, size=n)
    d = rng.uniform(-1.0, 1.0, size=n)
    e = rng.uniform(-1.0, 1.0, size=n)

    for _ in range(num_iterations):
        a = b * c + d / e - 3.14

    return a

# Vector size and number of iterations
n = 1000000
num_iterations = 100

# Trigger a warm-up run to compile the NumPy code
timer(lambda: vector_operations(100, 1))

# Perform the vector operations and measure the time
a = timer(lambda: vector_operations(n, num_iterations))