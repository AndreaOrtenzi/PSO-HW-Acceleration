# Particle Swarm Optimization - C - Fixed Point Arithmetic

This is a C implementation of the Particle Swarm Optimization algorithm using fixed point arithmetic.

## Fixed Point Arithmetic

The fixed point arithmetic is used in `main_fixedpoint.c`.
For a simpler implementation, look at `main.c`.

## Usage

Compile the code using the `Makefile`.

Run with
```
./main_fixedpoint <num_particles> <num_iterations> <lower_bound> <upper_bound>
```

Set the flags in `main_fixedpoint.c` to change the function to be optimized and whether to output the results to a file and to console during runtime.

## Results

Results can be written to `output.txt`.
The first line contains some information about the parameters used.
The following lines contain particle information for each iteration.

## Visualization

The results can be visualized using the Python script `visualize.py`.
It shows the animated PSO or saves a gif to `animation.gif`.

![pse_fn1](pso_fn1_100_100.gif)

![pse_fn2](pso_fn2_100_100.gif)

