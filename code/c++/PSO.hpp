#ifndef PSO_HPP
#define PSO_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
// #include <cstdlib>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

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
    PSO(const unsigned int                               &numParticles_,
        const unsigned int                               &dimensions_,
        const std::function<double(std::vector<double>)> &func_,
        const double                                     &upperBound_,
        const double                                     &lowerBound_,
        const double                                     &w_,
        const double                                     &c_             = 1.0,
        const double                                     &s_             = 1.0,
        const int                                        &maxIterations_ = 1000) :
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
     * @return std::vector<double>& that contains the best position.
     */
    std::vector<double> &
    getBestPosition();

    /**
     * @brief Get the best score.
     * @details This function returns the best score found by the algorithm.
     * @return double& that contains the best score.
     */
    double &
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
    std::function<double(std::vector<double>)> func;
    /**
     * @brief Inertia weight.
     */
    const double w;
    /**
     * @brief Cognitive weight coefficient.
     */
    const double c;
    /**
     * @brief Social weight coefficient.
     */
    const double s;
    /**
     * @brief Lower bound of the search space.
     */
    const double lowerBound;
    /**
     * @brief Upper bound of the search space.
     */
    const double upperBound;
    /**
     * @brief Maximum number of iterations to be performed.
     */
    const unsigned int maxIterations;
    /**
     * @brief Positions of the particles. For each particle, there is a vector of doubles that
     * contains its position in each dimension.
     */
    std::vector<std::vector<double>> positions;
    /**
     * @brief Best positions of the particles. For each particle, there is a vector of doubles that
     * contains its best position found so far in each dimension.
     */
    std::vector<std::vector<double>> bestPositions;
    /**
     * @brief Best position found by the algorithm so far.
     */
    std::vector<double> bestPosition;
    /**
     * @brief Scores of the particles. For each particle, there is a double that contains its score.
     */
    std::vector<double> scores;
    /**
     * @brief Best scores of the particles. For each particle, there is a double that contains its
     * best score found so far.
     */
    std::vector<double> bestScores;
    /**
     * @brief Best score found by the algorithm so far.
     */
    double bestScore;
    /**
     * @brief Velocities of the particles. For each particle, there is a vector of doubles that
     * contains its velocity in each dimension.
     */
    std::vector<std::vector<double>> velocities;
};

#endif