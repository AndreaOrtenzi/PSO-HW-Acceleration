# C++ implementation of the PSO algorithm.

Implementation of the Particle Swarm Optimization algorithm written in C++ using the standard library. The implementation follows the pseudo-code on the Wikipedia page of the [Particle Swarm Optimization](https://en.wikipedia.org/wiki/Particle_swarm_optimization#Algorithm) method.

## How to compile

To compile make sure you have installed CMake on you system, minimum version is 3.1.
Then, use the following commands:
```bash
mkdir build
cd build
cmake ..
make
```

## How to execute

The program can take as arguments the number of particles to use, the dimensions of the problem, the name of the function to minimize and the maximum number of iterations, in this order. These can be expanded if you need more flexibility on the parameters.
The functions implemented for now are the sphere function centered in the origin
```math
f(\mathbf{x}) = \sum_{i = 0}^d x_i^ \text{    ,}
```
where $d$ is the number of dimension of the problem, the global minimum is zero and occurs at $x_i = 0$ for all $i$, and the [Rosenbrock function](https://en.wikipedia.org/wiki/Rosenbrock_function)
```math
f(\mathbf{x}) = \sum_{i = 0}^{d-1} [100 (x_{i+1} - x_i^2)^2 + (1-x_i)^2] \text{    .}
```
that has a global minimum of zero in the middle of a plateau, that can be found at $x_i = 1$ for all $i$.

Also the function zoo can be expanded, as long as the structure follows the following functional form:
```cpp
std::function<double(std::vector<double>)> fct = ...
```
i.e., the function must take as input a C++ vector of doubles (all the the $x_i$), and return a single double ($`f(\mathbf{x})`$).

### Usage example
Example: execute the PSO algorithm with 5000 particles, on the 10-dimensional Rosenbrock function for 1000 iterations
```bash
./PSO 5000 10 rosenbrock 2000
```

# LITTLE SIDENOTE:
In the code, I manually set the seeds of the random number generators for reproducibility purpose: to remove these, search the comments that starts with `// NOTE:` comment the line where the seed is set, and uncomment the line where the seed sequence is set with the random device.