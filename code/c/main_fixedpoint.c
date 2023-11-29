#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <stdint.h>

#define W 0.75
#define PHI_P 1.0
#define PHI_G 1.0

// Run with parameters 10 100 -5.0 5.0, otherwise pass them as arguments:
#define DEBUG

// Generate output during iterations:
// #define VERBOSE
#define VERBOSE_INTERVAL 10

// Generate output for visualization:
#define OUTPUT
#define OUTPUT_INTERVAL 1

// Choose the cost function:
#define FUNCTION cost_func_1

///// FIXED POINT //////
// We're using s15.16 fixed point numbers:
// 1 bit for the sign
// 15 bits for the integer part
// 16 bits for the fractional part
#define FRAC_BITS 16


// Die with an error message
void die(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}

// Convert a double to a fixed point number
int32_t fp_from_double(double a) {
    return (int32_t)(a * (1 << FRAC_BITS));
}

// Convert a fixed point number to a double
double fp_to_double(int32_t a) {
    return ((double)a / (1 << FRAC_BITS));
}

// Multiplication requires a shift to the right by FRAC_BITS
int32_t fp_mul(int32_t a, int32_t b) {
    return (int32_t)(((long long)a * (long long)b) >> FRAC_BITS);
}

// Division requires a shift to the left by FRAC_BITS
int32_t fp_div(int32_t a, int32_t b) {
    return (int32_t)((((long long)a) << FRAC_BITS) / (long long)b);
}

// Generate a random fixed point number between v_lo and v_up
int32_t fp_rnd(int32_t v_lo, int32_t v_up) {
    // Generate a random number between 0 and 1
    double random_double = (double)rand() / (double)RAND_MAX;
    // Scale the random number to fit within the desired range [v_lo, v_up)
    int32_t random_fixed_point = fp_from_double(random_double) * ((v_up - v_lo) >> FRAC_BITS) + v_lo;
    return random_fixed_point;
}

// Cost function 1 (given)
int32_t cost_func_1(int32_t x, int32_t y) {
    int32_t z;
    z = fp_mul(x,x) + fp_mul(y,y);
    return z;
}

// Cost function 2 (given)
int32_t cost_func_2(int32_t x, int32_t y) {
    int32_t z;
    z = fp_mul(17039, (fp_mul(x,x) + fp_mul(y,y))) - fp_mul(31457, fp_mul(x,y));
    return z;
}

// Struct for the swarm
struct psoSwarm {
    int32_t best_pos[2];
    int32_t best_score;
};

// Struct for the particles
struct psoParticle {
    int32_t pos[2];
    int32_t vel[2];
    int32_t best_pos[2];
    int32_t best_score;
};

// Initialize the output file
FILE *init_output() {
    FILE *fp = fopen("output.txt", "w");
    if(fp == NULL)
        die("Error opening file output.txt\n");
    return fp;
}

// Write the state of the swarm to the output file
void write_state(FILE *fp, int iter, struct psoSwarm swarm, struct psoParticle *particles, int n_particles) {
    for(int i = 0; i < n_particles; ++i) {
        fprintf(fp, "%d\t%d\t%6.4f\t%6.4f\t%6.4f\t%6.4f\n",
            iter,
            i,
            fp_to_double(particles[i].pos[0]),
            fp_to_double(particles[i].pos[1]),
            fp_to_double(particles[i].vel[0]),
            fp_to_double(particles[i].vel[1])
        );
    }
    fprintf(fp, "%d\t%d\t%f\t%f\t%f\t%f\n",
        iter,
        -1,
        fp_to_double(swarm.best_pos[0]),
        fp_to_double(swarm.best_pos[1]),
        0.0,
        0.0
    );
    fprintf(fp, "%d\t%d\t%f\t%f\t%f\t%f\n",
        iter,
        -2,
        fp_to_double(swarm.best_score),
        0.0, 0.0, 0.0
    );
    return;
}


int main(int argc, char *argv[]) {

    // Set seed for random number generator (for reproducibility)
    srand(42);

    /////////////////////////////////////////////////
    // Set Parameters
    /////////////////////////////////////////////////

    #ifdef DEBUG
    int n_particles = 100;
    int n_iterations = 100;
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

    // Convert the parameters to fixed point
    int32_t fp_n_particles = fp_from_double(n_particles);
    int32_t fp_n_iterations = fp_from_double(n_iterations);
    int32_t fp_b_lo = fp_from_double(b_lo);
    int32_t fp_b_up = fp_from_double(b_up);

    int32_t fp_w = fp_from_double(W);
    int32_t fp_phi_p = fp_from_double(PHI_P);
    int32_t fp_phi_g = fp_from_double(PHI_G);

    // Output the parameters
    printf("+-----------------------------------------+\n");
    printf("|    Particle Swarm Optimization (FP)     |\n");
    printf("+-----------------------------------------+\n");
    printf("|              Fixed Point: | Int/Double: |\n");
    printf("|  n_particles:  %10.d | %10.f  |\n", fp_n_particles, fp_to_double(fp_n_particles));
    printf("|  n_iterations: %10.d | %10.f  |\n", fp_n_iterations, fp_to_double(fp_n_iterations));
    printf("|  b_lo:         %10.d |%11.2f  |\n", fp_b_lo, fp_to_double(fp_b_lo));
    printf("|  b_up:         %10.d |%11.2f  |\n", fp_b_up, fp_to_double(fp_b_up));
    printf("+-----------------------------------------+\n\n");

    /////////////////////////////////////////////////
    // Setup swarm and particles
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
    swarm.best_score = INT_MAX;


    /////////////////////////////////////////////////
    // Initialize the particles 
    /////////////////////////////////////////////////

    int32_t fp_vel_lo = fp_b_lo - fp_b_up;  // velocity lower bound
    int32_t fp_vel_up = - fp_vel_lo;        // velocity upper bound

    for(int i = 0; i < n_particles; ++i) {
        // Initialize the particle's position with a uniformly distributed random vector
        particles[i].pos[0] = fp_rnd(fp_b_lo, fp_b_up);
        particles[i].pos[1] = fp_rnd(fp_b_lo, fp_b_up);
        // Initialize the particle's best known position to its initial position
        particles[i].best_pos[0] = particles[i].pos[0];
        particles[i].best_pos[1] = particles[i].pos[1];
        // Calculate the particle's current score (which is also the best known score for now)
        particles[i].best_score = FUNCTION(particles[i].pos[0], particles[i].pos[1]);
        if(particles[i].best_score < swarm.best_score) {
            // Update the swarm's best known position
            swarm.best_pos[0] = particles[i].best_pos[0];
            swarm.best_pos[1] = particles[i].best_pos[1];
            swarm.best_score = particles[i].best_score;
        }
        // Initialize the particle's velocity: vi ~ U(-|bup-blo|, |bup-blo|)
        particles[i].vel[0] = fp_rnd(fp_vel_lo, fp_vel_up);
        particles[i].vel[1] = fp_rnd(fp_vel_lo, fp_vel_up);
    }

    // Print output for visualization
    #ifdef OUTPUT
    FILE *fp = init_output();
    fprintf(fp, "-1\t%d\t%d\t%d\t%f\t%f\n", FUNCTION == cost_func_1 ? 1 : 2, n_particles, n_iterations, b_lo, b_up);
    write_state(fp, 0, swarm, particles, n_particles);
    #endif

    #ifdef VERBOSE
    printf("Run Optimization Routine:\n\n");
    #endif

    /////////////////////////////////////////////////
    // Run the Optimization Routine
    /////////////////////////////////////////////////

    for(int iter= 0; iter < n_iterations; ++iter) {
        for(int i = 0; i < n_particles; ++i) {
            // Pick random numbers: rp, rg ~ U(0,1)
            int32_t rp = fp_from_double( ((double)rand() / RAND_MAX) );
            int32_t rg = fp_from_double( ((double)rand() / RAND_MAX) );
            // Update the particle's velocity: vi,d ← w vi,d + φp rp (pi,d-xi,d) + φg rg (gd-xi,d)
            particles[i].vel[0] =   fp_mul( fp_w, particles[i].vel[0] ) + 
                                    fp_mul( fp_mul(fp_phi_p, rp), (particles[i].best_pos[0] - particles[i].pos[0]) ) +
                                    fp_mul( fp_mul(fp_phi_g, rg), (swarm.best_pos[0] - particles[i].pos[0]) );
            // Pick random numbers: rp, rg ~ U(0,1)
            rp = fp_from_double( ((double)rand() / RAND_MAX) );
            rg = fp_from_double( ((double)rand() / RAND_MAX) );
            // Update the particle's velocity: vi,d ← w vi,d + φp rp (pi,d-xi,d) + φg rg (gd-xi,d)
            particles[i].vel[1] =   fp_mul( fp_w, particles[i].vel[1] ) + 
                                    fp_mul( fp_mul(fp_phi_p, rp), (particles[i].best_pos[1] - particles[i].pos[1]) ) +
                                    fp_mul( fp_mul(fp_phi_g, rg), (swarm.best_pos[1] - particles[i].pos[1]) );
            // Update the particle's position: xi ← xi + vi
            particles[i].pos[0] += particles[i].vel[0];
            particles[i].pos[1] += particles[i].vel[1];
            // if f(xi) < f(pi) then
            int32_t new_score = FUNCTION(particles[i].pos[0], particles[i].pos[1]);
            if(new_score < particles[i].best_score) {
                // Update the particle's best known position: pi ← xi
                particles[i].best_pos[0] = particles[i].pos[0];
                particles[i].best_pos[1] = particles[i].pos[1];
                particles[i].best_score = new_score;
                // if f(pi) < f(g) then
                if(new_score < swarm.best_score) {
                    // Update the swarm's best known position: g ← pi
                    swarm.best_pos[0] = particles[i].best_pos[0];
                    swarm.best_pos[1] = particles[i].best_pos[1];
                    swarm.best_score = new_score;
                }
            }
        }

        #ifdef VERBOSE
        if(iter % VERBOSE_INTERVAL == 0)
            printf("Iteration %d: %10.6f\n", iter, fp_to_double(swarm.best_score));
        #endif

        // Print output for visualization
        #ifdef OUTPUT
        if( (iter != 0) && (iter % OUTPUT_INTERVAL == 0) )
            write_state(fp, iter, swarm, particles, n_particles);
        #endif
    }


    /////////////////////////////////////////////////
    // Print Results
    /////////////////////////////////////////////////

    fprintf(stdout, "\n             Fixed Point: | Double:\n");
    fprintf(stdout, " Best score:   %10d | %10.6f\n", swarm.best_score, fp_to_double(swarm.best_score));
    fprintf(stdout, " Best position:\n");
    fprintf(stdout, "               %10d | %10.6f\n", swarm.best_pos[0], fp_to_double(swarm.best_pos[0]));
    fprintf(stdout, "               %10d | %10.6f\n", swarm.best_pos[1], fp_to_double(swarm.best_pos[1]));
    fprintf(stdout, "\n");

    free(particles);
    #ifdef OUTPUT
    fclose(fp);
    #endif

    return 0;
}