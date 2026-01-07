import random

# TODO : You can modify the function and argument below.
def estimate_pi(max_points):
    in_markers = 0
    pi = 0.
    # Generate random points and check if they are inside the unit circle
    for _ in range(max_points):
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        dis = x ** 2 + y ** 2
        if dis <= 1:
            in_markers += 1
    pi = 4 * (in_markers / max_points)
    return pi


# Modify the code to achieve an error rate below 0.1%.
# Print the estimated value of Pi.
print(estimate_pi(10000000))
