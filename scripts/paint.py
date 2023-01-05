import matplotlib.pyplot as plt
import sys
import os
import numpy as np

keys = ["iter", "x1", "x2", "f", "g1", "g2", "t", "c", "sigma"]


def rosenbrock(x, y):
    return (1 - x) ** 2 + 100 * (y - x**2) ** 2


def parse_log(log_file):
    data: dict = {key: [] for key in keys}
    with open(log_file, "r") as f:
        lines = f.readlines()
        # drop the first line
        lines = lines[1:]
        for line in lines:
            line = line.strip()
            line = line.split(",")
            for i, key in enumerate(keys):
                data[key].append(float(line[i]))
    return data


def paint(data, file_prefix):
    fig, ax = plt.subplots(2, 2, figsize=(10, 10))
    ax[0, 0].plot(data["x1"], data["x2"], "r-")
    x1, x2 = data["x1"], data["x2"]
    x1min, x1max = min(x1), max(x1)
    x1range = x1max - x1min
    x1min, x1max = x1min - x1range * 0.1, x1max + x1range * 0.1
    x2min, x2max = min(x2), max(x2)
    x2range = x2max - x2min
    x2min, x2max = x2min - x2range * 0.1, x2max + x2range * 0.1
    x1 = np.linspace(x1min, x1max, 100)
    x2 = np.linspace(x2min, x2max, 100)
    x1, x2 = np.meshgrid(x1, x2)
    z = rosenbrock(x1, x2)
    ax[0, 0].contour(x1, x2, z, 20, cmap="RdGy")
    ax[0, 0].plot(data["x1"][0], data["x2"][0], "go")
    # draw the last point in blue
    ax[0, 0].plot(data["x1"][-1], data["x2"][-1], "bo")
    ax[0, 0].set_title("x1-x2")
    ax[0, 0].set_xlabel("x1")
    ax[0, 0].set_ylabel("x2")
    ax[0, 1].plot(data["iter"], data["f"], "r-")
    ax[0, 1].set_title("iter-f")
    ax[0, 1].set_xlabel("iter")
    ax[0, 1].set_ylabel("f")
    ax[1, 0].plot(data["iter"], data["g1"], "r-")
    ax[1, 0].set_title("iter-g1")
    ax[1, 0].set_xlabel("iter")
    ax[1, 0].set_ylabel("g1")
    ax[1, 1].plot(data["iter"], data["g2"], "r-")
    ax[1, 1].set_title("iter-g2")
    ax[1, 1].set_xlabel("iter")
    ax[1, 1].set_ylabel("g2")
    plt.savefig(file_prefix + ".png")
    plt.close()


def main():
    log_file = sys.argv[1]
    print("log file: ", log_file)
    file_prefix = os.path.splitext(log_file)[0]
    data = parse_log(log_file)
    paint(data, file_prefix)


if __name__ == "__main__":
    main()
