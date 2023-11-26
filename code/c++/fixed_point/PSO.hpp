#ifndef PSO_HPP
#define PSO_HPP

#include "fpm/include/fpm/fixed.hpp"
#include "fpm/include/fpm/ios.hpp"
#include "fpm/include/fpm/math.hpp"

#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

/**
 * @brief Number of integer bits to be used.
 */
const unsigned int integer_bits = 16;
/**
 * @brief Number of fractional bits to be used.
 */
const unsigned int fractional_bits = 16;

/**
 * @brief Fixed-point type to be used.
 * Uses integer_bits bits for the integer part and fractional_bits bits for the fractional part.
 * Goes from -2^(integer_bits-1)+resolution to 2^(integer_bits-1)-resolution, with resolution of
 * 1/(2^fractional_bits).
 */
using fixed_double = fpm::fixed<std::int32_t, std::int64_t, fractional_bits>;

const fixed_double resolution{1. / (1 << fractional_bits)};

/**
 * @brief Maximum value of the fixed-point type.
 */
const fixed_double max_fixed{(1 << (integer_bits - 1)) - resolution};

/**
 * @brief Minimum value of the fixed-point type.
 */
const fixed_double min_fixed = -(max_fixed);

/**
 * @brief Resolution of the fixed-point type.
 */

/**
 * @brief Particle Swarm Optimization (PSO) algorithm.
 * @details This class implements the PSO algorithm. It can be used to minimize a function of
 * several variables.
 * @note This class is based on the pseudo-code found on the Wikipedia page for PSO:
 * https://en.wikipedia.org/wiki/Particle_swarm_optimization#Algorithm
 */
class PSO {
  public:
    /**
     * @brief Construct a new PSO object, which will be used to minimize the given function.
     * @details This constructor initializes the PSO object with the given parameters.
     *
     * @param numParticles_ Number of particles to use.
     * @param dimensions_ Number of dimensions of the search space.
     * @param func_ Function to minimize.
     * @param upperBound_ Upper bound of the search space.
     * @param lowerBound_ Lower bound of the search space.
     * @param w_ Inertia weight.
     * @param c_ Cognitive weight coefficient.
     * @param s_ Social weight coefficient.
     * @param maxIterations_ Maximum number of iterations.
     *
     *
     * @note The default values for c_ and s_ are 1.0, which means that the cognitive and social
     * components are weighted equally. These can be changed to tune the algorithm.
     */
    PSO(const unsigned int                                           &numParticles_,
        const unsigned int                                           &dimensions_,
        const std::function<fixed_double(std::vector<fixed_double>)> &func_,
        const fixed_double                                           &upperBound_,
        const fixed_double                                           &lowerBound_,
        const fixed_double                                           &w_,
        const fixed_double                                           &c_,
        const fixed_double                                           &s_,
        const int                                                    &maxIterations_ = 1000) :
        numParticles(numParticles_),
        dimensions(dimensions_), func(func_), upperBound(upperBound_), lowerBound(lowerBound_),
        w(w_), c(c_), s(s_), maxIterations(maxIterations_){};

    /**
     * @brief Initialize the particles.
     * @details This function initializes the particles by randomly generating their positions and
     * velocities. It also calculates the initial scores and sets the best scores and positions.
     * @note This function must be called before run().
     * @note The seed for the random number generator is hard-coded to 42 for reproducibility.
     * @see run()
     */
    void
    initializeParticles();

    /**
     * @brief Run the algorithm.
     * @details This function runs the algorithm. It updates the positions and velocities of the
     * particles, calculates the scores, and updates the best scores and positions.
     * @note This function must be called after initializeParticles().
     * @see initializeParticles()
     */
    void
    run();

    /**
     * @brief Get the best position.
     * @details This function returns the best position found by the algorithm.
     * @return std::vector<fixed_double>& that contains the best position.
     */
    std::vector<fixed_double> &
    getBestPosition();

    /**
     * @brief Get the best score.
     * @details This function returns the best score found by the algorithm.
     * @return fixed_double& that contains the best score.
     */
    fixed_double &
    getBestScore();

  private:
    /**
     * @brief Number of dimensions of the search space.
     */
    const unsigned int dimensions;
    /**
     * @brief Number of particles to use.
     */
    const unsigned int numParticles;
    /**
     * @brief Function to minimize.
     */
    std::function<fixed_double(std::vector<fixed_double>)> func;
    /**
     * @brief Inertia weight.
     */
    fixed_double w;
    /**
     * @brief Cognitive weight coefficient.
     */
    fixed_double c;
    /**
     * @brief Social weight coefficient.
     */
    fixed_double s;
    /**
     * @brief Lower bound of the search space.
     */
    const fixed_double lowerBound;
    /**
     * @brief Upper bound of the search space.
     */
    const fixed_double upperBound;
    /**
     * @brief Maximum number of iterations to be performed.
     */
    const unsigned int maxIterations;
    /**
     * @brief Positions of the particles. For each particle, there is a vector of doubles that
     * contains its position in each dimension.
     */
    std::vector<std::vector<fixed_double>> positions;
    /**
     * @brief Best positions of the particles. For each particle, there is a vector of doubles that
     * contains its best position found so far in each dimension.
     */
    std::vector<std::vector<fixed_double>> bestPositions;
    /**
     * @brief Best position found by the algorithm so far.
     */
    std::vector<fixed_double> bestPosition;
    /**
     * @brief Scores of the particles. For each particle, there is a double that contains its score.
     */
    std::vector<fixed_double> scores;
    /**
     * @brief Best scores of the particles. For each particle, there is a double that contains its
     * best score found so far.
     */
    std::vector<fixed_double> bestScores;
    /**
     * @brief Best score found by the algorithm so far.
     */
    fixed_double bestScore;
    /**
     * @brief Velocities of the particles. For each particle, there is a vector of doubles that
     * contains its velocity in each dimension.
     */
    std::vector<std::vector<fixed_double>> velocities;
};

#endif