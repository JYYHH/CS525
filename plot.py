import matplotlib.pyplot as plt
import numpy as np
import subprocess
import math


def run_matvecmul_script(script_name, num_processors, matrix_size=2048):
    command = ["mpirun", "-np", str(num_processors), script_name, "-w", str(matrix_size)]
    output = subprocess.check_output(command)
    return float(output.split(b"Elapsed time: ")[1].split(b" seconds")[0])


def plot1():
    num_processors = [i for i in range(1, 9)]

    rowwise_elapsed_times = []
    twod_elapsed_times = []

    for p in num_processors:
        rowwise_elapsed_time = run_matvecmul_script("1d", p)
        twod_elapsed_time = 100
        if p == 4:
            twod_elapsed_time = run_matvecmul_script("2dg", p)
        elif p % 2 == 0:
            twod_elapsed_time = run_matvecmul_script("2dc", p)
        else:
            twod_elapsed_time = run_matvecmul_script("1d", p)
        # twod_elapsed_time = run_matvecmul_script("matvecmul_2dpartition.py", p)
        rowwise_elapsed_times.append(rowwise_elapsed_time)
        twod_elapsed_times.append(twod_elapsed_time)

    # Calculate speedup and efficiency
    rowwise_speedup = [rowwise_elapsed_times[0] / t for t in rowwise_elapsed_times]
    twod_speedup = [twod_elapsed_times[0] / t for t in twod_elapsed_times]
    rowwise_efficiency = [s / p for s, p in zip(rowwise_speedup, num_processors)]
    twod_efficiency = [s / p for s, p in zip(twod_speedup, num_processors)]
    print(rowwise_speedup)
    print(rowwise_efficiency)
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(num_processors, rowwise_speedup, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(num_processors, twod_speedup, marker='o', label='2-D Partitioning')
    plt.xlabel('Number of Processors')
    plt.ylabel('Speedup')
    plt.title('Speedup vs Number of Processors')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./1-1.jpg")

    plt.figure(figsize=(10, 6))
    plt.plot(num_processors, rowwise_efficiency, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(num_processors, twod_efficiency, marker='o', label='2-D Partitioning')
    plt.xlabel('Number of Processors')
    plt.ylabel('Efficiency')
    plt.title('Efficiency vs Number of Processors')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./1-2.jpg")

def plot3():
    num_processors = [i for i in range(1, 9)]

    rowwise_elapsed_times = []
    twod_elapsed_times = []

    serial_times = []

    for p in num_processors:
        rowwise_elapsed_time = run_matvecmul_script("1d", p, 512*p)
        twod_elapsed_time = 100
        if p == 4 or p == 16:
            twod_elapsed_time = run_matvecmul_script("2dg", p, 512*p)
        elif p % 2 == 0:
            twod_elapsed_time = run_matvecmul_script("2dc", p, 512*p)
        else:
            twod_elapsed_time = run_matvecmul_script("1d", p, 512*p)
        # twod_elapsed_time = run_matvecmul_script("matvecmul_2dpartition.py", p)
        rowwise_elapsed_times.append(rowwise_elapsed_time)
        twod_elapsed_times.append(twod_elapsed_time)

        serial_time = run_matvecmul_script("1d", 1, 512*p)
        serial_times.append(serial_time)

    # Calculate speedup and efficiency
    rowwise_speedup = [st / t for st, t in zip(serial_times, rowwise_elapsed_times)]
    twod_speedup = [st / t for st, t in zip(serial_times, twod_elapsed_times)]
    rowwise_efficiency = [s / p for s, p in zip(rowwise_speedup, num_processors)]
    twod_efficiency = [s / p for s, p in zip(twod_speedup, num_processors)]
    print(rowwise_speedup)
    print(rowwise_efficiency)
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(num_processors, rowwise_speedup, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(num_processors, twod_speedup, marker='o', label='2-D Partitioning')
    plt.xlabel('Number of Processors')
    plt.ylabel('Speedup')
    plt.title('Speedup vs Number of Processors')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./3-1.jpg")

    plt.figure(figsize=(10, 6))
    plt.plot(num_processors, rowwise_efficiency, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(num_processors, twod_efficiency, marker='o', label='2-D Partitioning')
    plt.xlabel('Number of Processors')
    plt.ylabel('Efficiency')
    plt.title('Efficiency vs Number of Processors')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./3-2.jpg")

def plot2():

    rowwise_elapsed_times = []
    twod_elapsed_times = []
    serial_times = []

    matrix_sizes = [1<<exponent for exponent in range(9, 13)]
    for matrix_size in matrix_sizes:
        rowwise_elapsed_time = run_matvecmul_script("1d", 8, matrix_size)
        twod_elapsed_time = run_matvecmul_script("2dc", 8, matrix_size)
        rowwise_elapsed_times.append(rowwise_elapsed_time)
        twod_elapsed_times.append(twod_elapsed_time)
        serial_time = run_matvecmul_script("1d", 1, matrix_size)
        serial_times.append(serial_time)

    # Calculate speedup and efficiency
    rowwise_speedup = [st / t for st, t in zip(serial_times, rowwise_elapsed_times)]
    twod_speedup = [st / t for st, t in zip(serial_times, twod_elapsed_times)]
    rowwise_efficiency = [s / 8 for s in rowwise_speedup]
    twod_efficiency = [s / 8 for s in twod_speedup]
    print(rowwise_speedup)
    print(rowwise_efficiency)
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(matrix_sizes, rowwise_speedup, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(matrix_sizes, twod_speedup, marker='o', label='2-D Partitioning')
    plt.xlabel('Matrix Size')
    plt.ylabel('Speedup')
    plt.title('Speedup vs Matrix Size')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./2-1.jpg")

    plt.figure(figsize=(10, 6))
    plt.plot(matrix_sizes, rowwise_efficiency, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(matrix_sizes, twod_efficiency, marker='o', label='2-D Partitioning')
    plt.xlabel('Matrix Size')
    plt.ylabel('Efficiency')
    plt.title('Efficiency vs Matrix Size')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./2-2.jpg")

def plot4():
    num_processors1D = [i for i in range(1, 9)]
    num_processors2D = get_p_list()
    w_list = [i**2 for i in range(1, 9)]
    serial_times = []

    rowwise_elapsed_times = []
    twod_elapsed_times = []

    for idx in range(1, 9):
        serial_time = run_matvecmul_script("1d", 1, 512*idx)
        serial_times.append(serial_time)
        rowwise_elapsed_time = run_matvecmul_script("1d", idx, 512*(idx))
        twod_elapsed_time = 100
        if num_processors2D[idx-1] == 4:
            twod_elapsed_time = run_matvecmul_script("2dg", num_processors2D[idx-1], 512*idx)
        elif num_processors2D[idx-1] % 2 == 0:
            twod_elapsed_time = run_matvecmul_script("2dc", num_processors2D[idx-1], 512*idx)
        else:
            twod_elapsed_time = run_matvecmul_script("1d", num_processors2D[idx-1], 512*idx)
        # twod_elapsed_time = run_matvecmul_script("matvecmul_2dpartition.py", p)
        rowwise_elapsed_times.append(rowwise_elapsed_time)
        twod_elapsed_times.append(twod_elapsed_time)
        
    # Calculate speedup and efficiency
    rowwise_speedup = [st / t for st, t in zip(serial_times, rowwise_elapsed_times)]
    twod_speedup = [st / t for st, t in zip(serial_times, twod_elapsed_times)]
    rowwise_efficiency = [s / p for s, p in zip(rowwise_speedup, num_processors1D)]
    twod_efficiency = [s / p for s, p in zip(twod_speedup, num_processors2D)]
    print(rowwise_speedup)
    print(rowwise_efficiency)
    # Plotting
    plt.figure(figsize=(10, 6))
    plt.plot(w_list, rowwise_speedup, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(w_list, twod_speedup, marker='o', label='2-D Partitioning')
    plt.xlabel('Value of W')
    plt.ylabel('Speedup')
    plt.title('Speedup vs Value of W')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./4-1.jpg")

    plt.figure(figsize=(10, 6))
    plt.plot(w_list, rowwise_efficiency, marker='o', label='Row-wise 1-D Partitioning')
    plt.plot(w_list, twod_efficiency, marker='o', label='2-D Partitioning')
    plt.xlabel('Value of W')
    plt.ylabel('Efficiency')
    plt.title('Efficiency vs Value of W')
    plt.legend()
    plt.grid(True)
    # plt.show()
    plt.savefig("./4-2.jpg")

import math

def find_p(W):
    def target_function(p):
        logp = math.log2(p)
        return p * (logp ** 2)

    def binary_search(low, high):
        while low < high:
            mid = (low + high) / 2
            if target_function(mid) < W:
                low = mid + 0.01
            else:
                high = mid
        return low

    return binary_search(1, W)

def get_p_list(Ws=[1, 2**2, 3**2, 4**2, 5**2, 6**2, 7**2, 8**2]):
    ps = []
    for W in Ws:
        p = find_p(W)
        print("For W={}, the closest p is {}".format(W, p))
        print(p*((math.log2(p))**2))
        ps.append(math.ceil(p))
    print(ps)
    return ps


if __name__ == "__main__":
    plot4()
