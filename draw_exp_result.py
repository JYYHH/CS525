import matplotlib.pyplot as plt

X = list(range(1, 129))
Y_parallel = [30] * 128
Y_lazy = 30

with open("log.tmp", "r") as f:
    Index, sample_id = 0, 0
    for line in f:
        if line[:4] == "real":
            sample_id += 1
            if sample_id == 6:
                sample_id = 1
                Index += 1
            # Get min, most similar to the real time without system scheduling
            if Index == 128:
                Y_lazy = min(Y_lazy, float(line[-7:-2]))
            else:
                Y_parallel[Index] = min(Y_parallel[Index], float(line[-7:-2]))


plt.plot(X, Y_parallel, color='r', label='ParallelBT')
plt.plot(X, [Y_lazy] * 128, color='b', label='LazyBT')
plt.title("Execution Time vs. Number of Threads")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (seconds)")
plt.legend()
plt.savefig('img/perf.png')