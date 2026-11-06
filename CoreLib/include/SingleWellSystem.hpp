//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_SINGLEWELLSYSTEM_HPP
#define RECTQUANTUMWELLS_SINGLEWELLSYSTEM_HPP

#include <memory>

#include "QuantumSystem.hpp"
#include "SingleWellSolver.hpp"

/// Параметры одиночной прямоугольной ямы: V(x) = -depth при |x| <= halfWidth, иначе 0
struct SingleWellParameters {
    double depth; // V0 > 0
    double halfWidth; // a > 0, полная ширина ямы 2a
};

/**
 * @brief Одиночная прямоугольная квантовая яма.
 *
 * Объект неизменяемый: при смене параметров создаётся новый.
 * Недопустимые параметры (V0 <= 0, a <= 0) приводят к std::invalid_argument в конструкторе.
 */
class SingleWellSystem final : public QuantumSystem {
public:
    explicit SingleWellSystem(SingleWellParameters parameters,
                              std::shared_ptr<const SingleWellSolver> solver =
                                      std::make_shared<const SingleWellSolver>());

    [[nodiscard]] double potential(double x) const override;

    [[nodiscard]] std::vector<double> potentialDiscontinuities() const override;

    [[nodiscard]] SpatialRange potentialSupport() const override;

    [[nodiscard]] double potentialMinimum() const override;

    [[nodiscard]] std::vector<BoundState> boundStates() const override;

    [[nodiscard]] const SingleWellParameters &parameters() const { return m_parameters; }

private:
    SingleWellParameters m_parameters;
    std::shared_ptr<const SingleWellSolver> m_solver;
};

#endif //RECTQUANTUMWELLS_SINGLEWELLSYSTEM_HPP

