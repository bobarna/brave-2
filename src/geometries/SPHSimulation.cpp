#include "SPHSimulation.h"

#include <cmath>

SPHSimulation::SPHSimulation() : externalForces(.0f, .0f, .0f) {
    std::cout << "Initializing sph particle system with " << sph::N <<
              " particles." << std::endl;

    for (float y = 1; y < 5; y += 0.5f)
        for (float z = 0; z < 2; z += 0.2f)
            for (float x = 0; x < 2; x += 0.2f)
                if (particles.size() < sph::N) {
                    particles.emplace_back(
                            new Particle(
                                    vec3(util::randomBetween(-0.15f, 0.15f),
                                     util::randomBetween(0.3f, 0.35),
                                     util::randomBetween(-0.15f, 0.15f)),
                                    1.0,
                                    vec3(0.5f, 0.5f, 0.8f)
                            )
                    );
                }

    std::cout << "There are " << particles.size() << "particles." <<
              std::endl;
}

SPHSimulation::~SPHSimulation() {
    for (auto &particle : particles)
        delete particle;
}

void SPHSimulation::update(float t) {
    compute_density_and_pressure();
    compute_forces();

    integrate();
}

void SPHSimulation::Draw() {
    std::vector<float> particlePosAndColor;
    for (auto &p : particles) {
        particlePosAndColor.push_back(p->pos.x);
        particlePosAndColor.push_back(p->pos.y);
        particlePosAndColor.push_back(p->pos.z);
        particlePosAndColor.push_back(p->color.x);
        particlePosAndColor.push_back(p->color.y);
        particlePosAndColor.push_back(p->color.z);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // _STREAM_ indicates that the data will be modified, and then used only a few (one) time.
    // The buffer usage token might be changed to GL_DYNAMIC_DRAW when moving on to simulating positions on the GPU
    glBufferData(GL_ARRAY_BUFFER, particlePosAndColor.size() * sizeof(float), particlePosAndColor.data(),
                 GL_STREAM_DRAW);

    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) 0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void *) (sizeof(float) * 3));

    glPointSize(3.f);
    glDrawArrays(GL_POINTS, 0, particlePosAndColor.size() / 6);
}

void SPHSimulation::integrate() {
    for (auto &p : particles) {
        // forward euler
        p->v += sph::DT * p->f / p->rho;
        p->pos += sph::DT * p->v;

        const float max_b = 0.15f;
        const float min_b = -0.15f;

        //boundary conditions
        if (p->pos.x - sph::EPS <= min_b) {
            p->v.x *= sph::BOUND_DAMPING;
            p->pos.x = min_b + sph::EPS;
        }

        if (p->pos.x + sph::EPS > max_b) {
            p->v.x *= sph::BOUND_DAMPING;
            p->pos.x = max_b - sph::EPS;
        }

        // ground
        if (p->pos.y - sph::EPS <= 0.0f) {
            p->v.y *= sph::BOUND_DAMPING;
            p->pos.y = sph::EPS;
        }

        if (p->pos.y + sph::EPS >= 51) {
            p->v.y *= sph::BOUND_DAMPING;
            p->pos.y = 51 - sph::EPS;
        }

        //TODO DEPTH CONSTANT VALUE
        if (p->pos.z - sph::EPS <= min_b) {
            p->v.z *= sph::BOUND_DAMPING;
            p->pos.z = min_b + sph::EPS;
        }

        if (p->pos.z + sph::EPS >= max_b) {
            p->v.z *= sph::BOUND_DAMPING;
            p->pos.z = max_b - sph::EPS;
        }

        p->v *= 0.98;
    }

}

void SPHSimulation::compute_forces() {
    for (auto &p_i : particles) {
        vec3 f_pressure(0.f, 0.f, 0.f);
        vec3 f_viscosity(0.f, 0.f, 0.f);

        for (auto &p_j : particles) {
            if (&p_i == &p_j) continue;

            vec3 r_ijv = p_i->pos - p_j->pos;
            float r_ij = length(r_ijv);

            if (r_ij <= sph::H) {
                // compute pressure force contribution

                f_pressure += -sph::M * (p_i->p + p_j->p) / (2 * p_j->rho) *
                              sph::POLY6_GRAD_PRESS * r_ijv / r_ij * std::pow(sph::H - r_ij, 2.0f);

                f_viscosity += sph::MU * sph::M * (p_j->v - p_i->v) / p_j->rho *
                               sph::POLY6_GRAD_VISC * (sph::H - r_ij);
            }

            vec3 f_gravity = sph::G * p_i->rho;

            p_i->f = f_pressure + f_viscosity + f_gravity;
            p_i->f = f_gravity + f_pressure;// * 0.001f;
        }
    }
}

void SPHSimulation::compute_density_and_pressure() {
    for (auto &p_i : particles) {
        // compute density
        p_i->rho = 0.f;
        for (auto &p_j : particles) {
            vec3 r_ij = p_j->pos - p_i->pos;
            // euclidean distance between p_i and p_j
            float r = length(p_j->pos - p_i->pos);
            float r2 = std::pow(r, 2.f);

            // 0 <= r <= d
            if (r2 <= sph::H2)
                p_i->rho += sph::M * sph::POLY6 * std::pow(sph::H2 - r2, 3.f);
        }

        // compute pressure
        p_i->p = sph::GAS_CONST * (p_i->rho - sph::REST_DENS);
    }

}

vec3 SPHSimulation::getExternalForces() const {
    return vec3();
}

void SPHSimulation::resetExternalForces() {
    externalForces = vec3(0, 0, 0);
}

void SPHSimulation::addForce(vec3 force) {
    externalForces += force;
}
