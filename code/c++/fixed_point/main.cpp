#include "PSO.hpp"

#include <iomanip>

int
main(int argc, char **argv) {
    // Create a PSO object with 2 dimensions, 100 particles, and a function
    // to optimize.

    std::cout << "===!!!=== NOTE: " << std::endl
              << "This program uses fixed-point arithmetic to perform the calculations."
              << std::endl;
    std::cout << "Maximum value of the fixed-point type: " << max_fixed << std::endl;
    std::cout << "Minimum value of the fixed-point type: " << min_fixed << std::endl;
    std::cout << "Resolution of the fixed-point type: " << resolution << std::endl;
    std::cout << "===!!!=== END of NOTE: " << std::endl << std::endl;
    std::cout << "==================================================================" << std::endl;

    unsigned int                                           numParticles  = 100;
    unsigned int                                           dimensions    = 2;
    unsigned int                                           maxIterations = 1000;
    fixed_double                                           lowerBound{-5.0};
    fixed_double                                           upperBound{5.0};
    std::function<fixed_double(std::vector<fixed_double>)> rosenbrock =
        [](std::vector<fixed_double> x) {
            fixed_double sum{0.0};
                for (int i = 0; i < x.size() - 1; i++) {
                    sum += 100 * fpm::pow(x[i + 1] - fpm::pow(x[i], 2), 2) + fpm::pow(1 - x[i], 2);
                }
            return sum;
        };
    std::function<fixed_double(std::vector<fixed_double>)> f1 = [](std::vector<fixed_double> x) {
        fixed_double sum{0.0};
            for (int i = 0; i < x.size(); i++) {
                sum += fpm::pow(x[i], 2);
            }
        return sum;
    };
    std::function<fixed_double(std::vector<fixed_double>)> f2 = [](std::vector<fixed_double> x) {
        fixed_double sum{0.0};
        fixed_double c1{0.26};
        fixed_double c2{0.48};
        sum += c1 * (fpm::pow(x[0], 2) + fpm::pow(x[1], 2)) - c2 * x[0] * x[1];
        return sum;
    };
    std::function<fixed_double(std::vector<fixed_double>)> f = f1;

        if (argc != 5) {
            fixed_double lowerBound{-5.0};
            fixed_double upperBound{5.0};
            std::cout << "SETTING DEFAULT PARAMETERS:" << std::endl;
            std::cout << "Number of particles: " << numParticles << std::endl;
            std::cout << "Dimensions: " << dimensions << std::endl;
            std::cout << "Function to minimize: function 1" << std::endl;
            std::cout << "Max number of iterations: " << maxIterations << std::endl;
            std::cout << "=====> If you want to change these, please read the correct usage below."
                      << std::endl;
            std::cout << "Usage: " << argv[0]
                      << " [numParticles] [dimensions] [function] [maxIterations]" << std::endl;
            std::cout << "Available functions: rosenbrock, f1, f2" << std::endl;
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
                } else if (function.compare("f1") == 0) {
                    f = f1;
                    std::cout << "Function chosen: function 1." << std::endl;
                } else if (function.compare("f2") == 0) {
                    f          = f2;
                    dimensions = 2;
                    lowerBound = fixed_double{-10.0};
                    upperBound = fixed_double{10.0};
                    std::cout << "Function chosen: function 2." << std::endl;
                } else {
                    f = f1;
                    std::cout << "Function not recognized. Using function 1." << std::endl;
                }
            std::cout << "Max number of iterations: " << maxIterations << std::endl;
        }

    fixed_double w{0.75};
    fixed_double c{1.0};
    fixed_double s{1.0};

    std::cout << "------------------------------------------------------------------" << std::endl;
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