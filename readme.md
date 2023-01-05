# Numerical Optimization in Robotics

This repository contains the code for the course "Numerical Optimization in Robotics" at Shenlanxueyuan. All the code is written in C++ and Python, and has been tested on Ubuntu 20.04.

## Compile and Run built-in examples

```bash
./scripts/configure.sh -t all
```

The above command will compile the code and run the examples written in coressponding source files. You can also run the examples by yourself, following the instructions below for each homework.

## Homework 1: Linear-search Steepest Gradient Descent

The executable file is `build/lsgd/lsgd`. To run the program, you have to provide the following arguments:
1. log file path.
2. problem dimension.

For example, to run the program with a 10-dimensional problem, you can run the following command:
```bash
./build/lsgd/lsgd ./build/lsgd/log.txt 10
```

The test case of dimension 2 is provided in `scripts/test.sh`. It will be called in `scripts/configure.sh`, write results in `data/yyyy-mm-dd-hh-mm-ss.log`, and plot the result in `data/yyyy-mm-dd-hh-mm-ss.png`.