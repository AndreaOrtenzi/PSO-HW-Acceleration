#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define W 0.75
#define PHI_P 1.0
#define PHI_G 1.0

// #define DEBUG


struct psoSwarm {
    double best_pos[2];
    double best_score;
};


struct psoParticle {
    double pos[2];
    double vel[2];
    double best_pos[2];
    double best_score;
};

double cost_func_1(double x, double y) {
    // range: -5.0, 5.0
    double z;
    z = x*x + y*y;
    return z;
}

double cost_func_2(double x, double y) {
    // range: -10.0, 10.0
    double z;
    z = 0.26 * (x*x + y*y) - 0.48 * x * y;
    return z;
}


int main(int argc, char *argv[]) {

    srand(42);

    /////////////////////////////////////////////////
    // Set Parameters
    /////////////////////////////////////////////////
    #ifdef DEBUG
    int n_particles = 100;
    int n_iterations = 10000;
    double b_lo = -5.0;
    double b_up = 5.0;
    #else
    if(argc != 5) {
        printf("Usage:\n");
        printf("%s <n_particles> <n_iterations> <domain_lower_bound> <domain_upper_bound>\n", argv[0]);
        return 1;
    }

    int n_particles = atoi(argv[1]);
    int n_iterations = atoi(argv[2]);
    double b_lo = atof(argv[3]);
    double b_up = atof(argv[4]);
    #endif

    printf("+-----------------------------------------+\n");
    printf("|       Particle Swarm Optimization       |\n");
    printf("+-----------------------------------------+\n");
    printf("|  n_particles:  %10.d               |\n", n_particles);
    printf("|  n_iterations: %10.d               |\n", n_iterations);
    printf("|  b_lo:        %11.4f               |\n", b_lo);
    printf("|  b_up:        %11.4f               |\n", b_up);
    printf("+-----------------------------------------+\n\n");


    /////////////////////////////////////////////////
    // Setup
    /////////////////////////////////////////////////

    // Create the particles
    struct psoParticle *particles;
    particles = (struct psoParticle *) calloc(n_particles, sizeof(struct psoParticle));
    if(particles == NULL) {
        printf("Error allocating memory\n");
        return 1;
    }

    // Create the swarm
    struct psoSwarm swarm;
    for(int i = 0; i < 2; ++i) {
        swarm.best_pos[i] = b_lo;
    }
    swarm.best_score = DBL_MAX;


    /////////////////////////////////////////////////
    // initialize the particles 
    /////////////////////////////////////////////////

    double vel_low = b_lo - b_up;
    double vel_up = - vel_low;

    for(int i = 0; i < n_particles; ++i) {
        for(int j = 0; j < 2; ++j) {
            // Initialize the particle's position with a uniformly distributed random vector
            particles[i].pos[j] = ((double)rand() / RAND_MAX) * (b_up - b_lo) + b_lo;
            // Initialize the particle's best known position to its initial position
            particles[i].best_pos[j] = particles[i].pos[j];
            // Calculate the particle's current score (which is also the best known score for now)
            particles[i].best_score = cost_func_2(particles[i].pos[0], particles[i].pos[1]);
            if(particles[i].best_score < swarm.best_score) {
                // Update the swarm's best known position
                swarm.best_pos[0] = particles[i].best_pos[0];
                swarm.best_pos[1] = particles[i].best_pos[1];
                swarm.best_score = particles[i].best_score;
            }
            // Initialize the particle's velocity: vi ~ U(-|bup-blo|, |bup-blo|)
            particles[i].vel[j] = ((double)rand() / RAND_MAX) * (vel_up - vel_low) + vel_low;
        }
    }


    /////////////////////////////////////////////////
    // Run the Optimization Routine
    /////////////////////////////////////////////////

    for(int iter= 0; iter < n_iterations; ++iter) {
        for(int i = 0; i < n_particles; ++i) {
            for(int j = 0; j < 2; ++j) {
                // Pick random numbers: rp, rg ~ U(0,1)
                double rp = ((double)rand() / RAND_MAX);
                double rg = ((double)rand() / RAND_MAX);
                // Update the particle's velocity: vi,d ← w vi,d + φp rp (pi,d-xi,d) + φg rg (gd-xi,d)
                particles[i].vel[j] =   W * particles[i].vel[j] + 
                                        PHI_P * rp * (particles[i].best_pos[j] - particles[i].pos[j]) +
                                        PHI_G * rg * (swarm.best_pos[j] - particles[i].pos[j]);
            }
            // Update the particle's position: xi ← xi + vi
            for(int j = 0; j < 2; ++j)
                particles[i].pos[j] += particles[i].vel[j];
            // if f(xi) < f(pi) then
            double new_score = cost_func_2(particles[i].pos[0], particles[i].pos[1]);
            if(new_score < particles[i].best_score) {
                // Update the particle's best known position: pi ← xi
                for(int j = 0; j < 2; ++j) {
                    particles[i].best_pos[j] = particles[i].pos[j];
                }
                particles[i].best_score = new_score;
                // if f(pi) < f(g) then
                if(new_score < swarm.best_score) {
                    // Update the swarm's best known position: g ← pi
                    for(int j = 0; j < 2; ++j)
                        swarm.best_pos[j] = particles[i].best_pos[j];
                    swarm.best_score = new_score;
                }
            }
        }
    }


    /////////////////////////////////////////////////
    // Print Results
    /////////////////////////////////////////////////

    fprintf(stdout, "Best score: %f\n", swarm.best_score);
    fprintf(stdout, "Best position: ");
    for(int j = 0; j < 2; ++j) {
        fprintf(stdout, "%f ", swarm.best_pos[j]);
    }
    fprintf(stdout, "\n");

    free(particles);

    return 0;
}