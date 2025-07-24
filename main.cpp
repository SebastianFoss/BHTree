#include <iostream>
#include <random>

#include "BHtree.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
int main() {
    std::cout << "BHTree driver v1.0" << std::endl;
    std::unique_ptr<BHtree> tree;

    const int NUM_PARTICLES = 1000;
    const double sim_time = 1000.0;
    const double dt = sim_time / NUM_PARTICLES;
    const double THETA = 0.5;

    const double RANGE = 1000.0;

    std::vector<Particle> particles;
    particles.reserve(NUM_PARTICLES);

    std::random_device rd;
    std::mt19937 gen(rd());
    // Distribution for positions (e.g., within a cube)
    std::uniform_real_distribution<> pos_dist(-RANGE / 2.0, RANGE / 2.0);
    // Distribution for velocities (start mostly at rest with some small random motion)
    std::uniform_real_distribution<> vel_dist(-0.1, 0.1);
    // Distribution for mass
    std::uniform_real_distribution<> mass_dist(1e10, 1e12); // Representative stellar masses

    std::cout << "Generating " << NUM_PARTICLES << " particles..." << std::endl;
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        Vec pos(pos_dist(gen), pos_dist(gen), pos_dist(gen));
        Vec vel(vel_dist(gen), vel_dist(gen), vel_dist(gen));
        double mass = mass_dist(gen);
        particles.emplace_back(pos, vel, Vec(), mass, i);
    }
    std::cout << "Particles generated." << std::endl;

    // --- Initialize BHtree ---
    std::unique_ptr<BHtree> bhtree;
    try {
        bhtree = std::make_unique<BHtree>(std::move(particles)); // Pass by rvalue reference
        std::cout << "BHtree initialized." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error initializing BHtree: " << e.what() << std::endl;
        return 1;
    }

    try {
        bhtree->buildTree();
    }
    catch (const std::exception& e) {
        std::cerr << "Error building tree: " << e.what() << std::endl;
    }
}
//
//     // --- Simulation Loop ---
//     int num_steps = static_cast<int>(SIMULATION_TIME / DT);
//     std::cout << "Running simulation for " << num_steps << " steps with dt = " << DT << std::endl;
//
//     for (int step = 0; step < num_steps; ++step) {
//         if (step % 100 == 0) {
//             std::cout << "Simulation Step: " << step << " / " << num_steps << std::endl;
//             // Optionally print state of a few particles periodically
//             // For example, print the first particle:
//             // if (bhtree->particles[0]) {
//             //     printParticleState(*bhtree->particles[0], step);
//             // }
//         }
//
//         auto start_step_time = std::chrono::high_resolution_clock::now();
//
//         // Perform one simulation step (build tree, calculate forces, update particles)
//         try {
//             bhtree->step(DT, THETA);
//         } catch (const std::exception& e) {
//             std::cerr << "Error during simulation step " << step << ": " << e.what() << std::endl;
//             break; // Exit loop on error
//         }
//
//         auto end_step_time = std::chrono::high_resolution_clock::now();
//         std::chrono::duration<double> step_duration = end_step_time - start_step_time;
//         // std::cout << "Step " << step << " took: " << step_duration.count() * 1000.0 << " ms" << std::endl;
//     }
//
//     std::cout << "Simulation finished." << std::endl;
// }