//
// Created by sailsec on 7/7/25.
//

#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec.h" // Ensure Vec is properly defined and accessible

// The Particle class represents a single body in this N-body simulation.
// It holds its physical properties and manages its state changes over time.

class Particle {
private:
    Vec pos;          // Position vector
    Vec vel;          // Velocity vector
    Vec acc;          // Accumulated acceleration vector (sum of forces / mass)
    double mass;      // Mass of the particle
    int id;           // Unique identifier for the particle (useful for debugging/tracking)

    double potential_phi;
    // accumulated potential

public:
    // Constructor: Initializes a particle with its properties.
    // Provides default values for velocity, acceleration, and ID for convenience.
    Particle(Vec pos_ = Vec(), Vec vel_ = Vec(), Vec acc_ = Vec(), double mass_ = 1.0, int id_ = 0)
        : pos(pos_), vel(vel_), acc(acc_), mass(mass_), id(id_) {
        // No heap allocation needed for Vec members, they are value types
    }

    // --- Getters (const-correctness and reference returns for efficiency) ---
    const Vec& getPos() const { return pos; }
    const Vec& getVel() const { return vel; }
    const Vec& getAcc() const { return acc; }
    double getMass() const { return mass; }
    int getId() const { return id; }

    // --- Methods for force accumulation and state update ---

    // Resets the accumulated acceleration (or force) for the current time step.
    // Called at the beginning of each force calculation phase.
    void resetAccumulatedForce() {
        acc = Vec(); // Set acceleration vector to (0,0,0)
    }

    // Adds a force vector to the particle's accumulated acceleration.
    // Internally converts force to acceleration (F/m).
    void addAccumulatedForce(const Vec& force) {
        if (mass > 0) {
            acc = acc + (force / mass); // F = ma, so a = F/m
        }
        // If mass is zero, force has no effect on acceleration (or handle as error if appropriate)
    }

    // Updates the particle's position and velocity based on its accumulated acceleration
    // and the given time step (dt). Implements simple Euler-Cromer integration.
    void update(double dt) {
        vel = vel + (acc * dt); // Update velocity based on acceleration
        pos= pos + (vel * dt); // Update position based on new velocity

        // Reset acceleration for the next time step's force accumulation.
        acc = Vec();
    }

    // ADDED: Resets the accumulated potential for the current time step.
    void resetPotentialPhi() {
        potential_phi = 0.0;
    }

    // ADDED: Adds a scalar potential value to the particle's accumulated potential.
    void addPotentialPhi(double phi) {
        potential_phi += phi;
    }
};

#endif //PARTICLE_H