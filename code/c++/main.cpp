#include "PSO.hpp"

int
main(int argc, char **argv) {
    // Create a PSO object with 2 dimensions, 100 particles, and a function
    // to optimize.

    int                                        numParticles  = 100;
    int                                        dimensions    = 2;
    int                                        maxIterations = 1000;
    std::function<double(std::vector<double>)> rosenbrock    = [](std::vector<double> x) {
        double sum = 0.0;
            for (int i = 0; i < x.size() - 1; i++) {
                sum += 100.0 * std::pow(x[i + 1] - std::pow(x[i], 2), 2) + std::pow(1.0 - x[i], 2);
            }
        return sum;
    };
    std::function<double(std::vector<double>)> sphere = [](std::vector<double> x) {
        double sum = 0.0;
            for (int i = 0; i < x.size(); i++) {
                sum += std::pow(x[i], 2);
            }
        return sum;
    };
    std::function<double(std::vector<double>)> f = sphere;

        if (argc != 5) {
            std::cout << "SETTING DEFAULT PARAMETERS:" << std::endl;
            std::cout << "Number of particles: " << numParticles << std::endl;
            std::cout << "Dimensions: " << dimensions << std::endl;
            std::cout << "Function to minimize: sphere" << std::endl;
            std::cout << "Max number of iterations: " << maxIterations << std::endl;
            std::cout
                << "=========> If you want to change these, please read the correct usage below."
                << std::endl;
            std::cout << "Usage: " << argv[0]
                      << " [numParticles] [dimensions] [function] [maxIterations]" << std::endl;
        } else {
            numParticles         = std::stoi(argv[1]);
            dimensions           = std::stoi(argv[2]);
            std::string function = argv[3];
            maxIterations        = std::stoi(argv[4]);

            std::cout << "PARSED PARAMETERS:" << std::endl;
            std::cout << "Number of particles: " << numParticles << std::endl;
            std::cout << "Dimensions: " << dimensions << std::endl;
                if (function.compare("rosenbrock") == 0) {
                    f = rosenbrock;
                    std::cout << "Function chosen: Rosenbrock function." << std::endl;
                } else if (function.compare("sphere") == 0) {
                    f = sphere;
                    std::cout << "Function chosen: sphere function." << std::endl;
                } else {
                    std::cout << "Function not recognized. Using sphere function." << std::endl;
                    f = sphere;
                }
            std::cout << "Max number of iterations: " << maxIterations << std::endl;
        }

    double lowerBound = -5.0;
    double upperBound = 5.0;
    double w          = 0.75;
    double c          = 1.0;
    double s          = 1.0;

    std::cout << "---------------------------------------------------------------" << std::endl;
    PSO pso = PSO(numParticles, dimensions, f, upperBound, lowerBound, w, c, s, maxIterations);
    pso.initializeParticles();
    pso.run();
    std::cout << "Best score: " << pso.getBestScore() << std::endl;
    std::cout << "Best position: ";
        for (auto x : pso.getBestPosition()) {
            std::cout << x << " ";
        }
    std::cout << std::endl;

    return 0;
}