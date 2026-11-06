//
// Created by Mikhail on 06.11.2026.
//

#include "SingleWellSystem.hpp"

#include <cmath>
#include <stdexcept>

SingleWellSystem::SingleWellSystem(SingleWellParameters parameters,
                                   std::shared_ptr<const SingleWellSolver> solver)
    : m_parameters(parameters), m_solver(std::move(solver)) {
    if (!m_solver) {
        throw std::invalid_argument("Solver cannot be null");
    }
    // Форма !(x > 0) отсекает и NaN
    if (!(m_parameters.depth > 0.0)) {
        throw std::invalid_argument("Well depth V0 must be positive");
    }
    if (!(m_parameters.halfWidth > 0.0)) {
        throw std::invalid_argument("Well half-width a must be positive");
    }
}

double SingleWellSystem::potential(const double x) const {
    return std::abs(x) <= m_parameters.halfWidth ? -m_parameters.depth : 0.0;
}

std::vector<double> SingleWellSystem::potentialDiscontinuities() const {
    return {-m_parameters.halfWidth, m_parameters.halfWidth};
}

SpatialRange SingleWellSystem::potentialSupport() const {
    return {-m_parameters.halfWidth, m_parameters.halfWidth};
}

double SingleWellSystem::potentialMinimum() const {
    return -m_parameters.depth;
}

std::vector<BoundState> SingleWellSystem::boundStates() const {
    const auto levels = m_solver->findLevels(m_parameters.depth, m_parameters.halfWidth);

    std::vector<BoundState> states;
    states.reserve(levels.size());
    for (const auto &level: levels) {
        states.push_back({level, m_solver->getWaveFunction(level, m_parameters.depth, m_parameters.halfWidth)});
    }
    return states;
}

