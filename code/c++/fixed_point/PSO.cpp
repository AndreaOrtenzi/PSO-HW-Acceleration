#include "PSO.hpp"

std::vector<fixed_double> &
PSO::getBestPosition() {
    return bestPosition;
}

fixed_double &
PSO::getBestScore() {
    return bestScore;
}

void
PSO::initializeParticles() {
    std::cout << "==================================================================" << std::endl;
    std::cout << "===== Initializing particles..." << std::endl;
    // NOTE: I hard-coded the seed to 42 for reproducibility.
    // std::random_device               rd;
    std::mt19937                     gen(42);
    std::uniform_real_distribution<> posDis(static_cast<double>(lowerBound),
                                            static_cast<double>(upperBound));
    std::uniform_real_distribution<> velDis(static_cast<double>(lowerBound - upperBound),
                                            static_cast<double>(upperBound - lowerBound));
    fixed_double                     score;
    bestScore = max_fixed;

        for (int i = 0; i < numParticles; i++) {
            std::vector<fixed_double> position;
            std::vector<fixed_double> velocity;
                for (int j = 0; j < dimensions; j++) {
                    fixed_double posFixed{posDis(gen)};
                    position.push_back(posFixed);
                    fixed_double velFixed{posDis(gen)};
                    velocity.push_back(velFixed);
                }
            positions.push_back(position);
            velocities.push_back(velocity);
            bestPositions.push_back(position);
            score = func(position);
            scores.push_back(score);
                if (score < bestScore) {
                    bestScore    = score;
                    bestPosition = position;
            }
            bestScores.push_back(max_fixed);
        }
    std::cout << "===== Initialization done" << std::endl;
}

void
PSO::run() {
    std::cout << "==================================================================" << std::endl;
    std::cout << "===== Starting the algorithm..." << std::endl;
    // NOTE: I hard-coded the seed to 42 for reproducibility.
    // std::random_device               rd;
    std::mt19937                     gen(42);
    std::uniform_real_distribution<> dis(0.0, 1.0);
    unsigned int                     iterBetweenPrints = std::floor(maxIterations / 10);
    double                           totalTime;
    double                           timeUntilBest;
    unsigned int                     bestIter;

    {
        using namespace std::chrono;
        const auto start   = high_resolution_clock::now();
        auto       endBest = high_resolution_clock::now();
            for (int i = 0; i < maxIterations; i++) {
                    if (i % iterBetweenPrints == 0) {
                        std::string bestPositionString = "";
                            for (int d = 0; d < dimensions; d++) {
                                bestPositionString +=
                                    std::to_string(static_cast<double>(bestPosition[d])) + " ";
                            }
                        std::cout << "Iteration: " << i << ", Best score: " << bestScore
                                  << ", Best position: " << bestPositionString << std::endl;
                }
                    for (int j = 0; j < numParticles; j++) {
                            for (int d = 0; d < dimensions; d++) {
                                fixed_double rc{dis(gen)};
                                fixed_double rs{dis(gen)};
                                velocities[j][d] =
                                    w * velocities[j][d] +
                                    c * rc * (bestPositions[j][d] - positions[j][d]) +
                                    s * rs * (bestPosition[d] - positions[j][d]);
                                positions[j][d] += velocities[j][d];
                            }
                        scores[j] = func(positions[j]);
                            if (scores[j] < bestScores[j]) {
                                bestScores[j]    = scores[j];
                                bestPositions[j] = positions[j];
                                    if (bestScores[j] < bestScore) {
                                        bestScore    = bestScores[j];
                                        bestPosition = bestPositions[j];
                                        endBest      = high_resolution_clock::now();
                                        bestIter     = i;
                                }
                        }
                    }
            }
        const auto end = high_resolution_clock::now();
        totalTime      = duration_cast<milliseconds>(end - start).count();
        timeUntilBest  = duration_cast<milliseconds>(endBest - start).count();
    }
    std::cout << "===== Algorithm finished!" << std::endl;
    std::cout << "Total time: " << totalTime << " ms" << std::endl;
    std::cout << "Time until best: " << timeUntilBest << " ms" << std::endl;
    std::cout << "Best iteration: " << bestIter << std::endl;
}
