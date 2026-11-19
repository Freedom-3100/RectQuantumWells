//
// Created by Mikhail on 06.11.2026.
//

#include "SingleWellSolver.hpp"

#include "RootBracketFinder.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {
    double validatedMassRatio(const double effectiveMassRatio) {
        if (effectiveMassRatio <= 0.0) {
            throw std::invalid_argument("Effective mass ratio must be positive");
        }
        return effectiveMassRatio;
    }
}

SingleWellSolver::SingleWellSolver(Constants::UnitSystem unitSystem,
                                   double effectiveMassRatio,
                                   std::shared_ptr<EqSolver<double> > rootSolver,
                                   std::shared_ptr<IntegralCalculator<double> > integrator)
    : m_unitSystem(unitSystem),
      m_effectiveMassRatio(validatedMassRatio(effectiveMassRatio)),
      m_gamma(Constants::getHbar2Div2M(unitSystem, m_effectiveMassRatio)),
      m_rootSolver(std::move(rootSolver)),
      m_integrator(std::move(integrator)) {
    if (!m_rootSolver || !m_integrator) {
        throw std::invalid_argument("Solvers cannot be null");
    }
}

std::vector<EnergyLevel> SingleWellSolver::findLevels(double V0,
                                                      double a,
                                                      double scanStep,
                                                      double iTolerance,
                                                      size_t maxStepsCount) const {
    if (V0 <= 0.0 || a <= 0.0 || iTolerance <= 0.0) {
        return {};
    }

    const double gamma = m_gamma;
    const double R = std::sqrt(V0 * a * a / gamma);

    // Если яма настолько мелкая/узкая, что R меньше толеранса
    if (R <= 2.0 * iTolerance) {
        return {};
    }

    auto fEven = [R](const double u) {
        return u * std::sin(u) - std::sqrt(R * R - u * u) * std::cos(u);
    };

    auto fOdd = [R](const double u) {
        return u * std::cos(u) + std::sqrt(R * R - u * u) * std::sin(u);
    };

    std::vector<EnergyLevel> levels;

    const double leftBound = iTolerance;
    const double rightBound = R - iTolerance;

    // Чётные состояния
    for (auto intervalsEven = RootBracketFinder<double>::findIntervals(fEven, leftBound, rightBound, scanStep); const
         auto &[left, right]: intervalsEven) {
        if (auto root = m_rootSolver->solve(fEven, left, right, iTolerance, maxStepsCount)) {
            const double u = *root;
            const double E = -V0 + gamma * (u * u) / (a * a);
            levels.push_back({0, E, true});
        }
    }

    // Нечётные состояния
    for (auto intervalsOdd = RootBracketFinder<double>::findIntervals(fOdd, leftBound, rightBound, scanStep); const auto
         &[left, right]: intervalsOdd) {
        if (auto root = m_rootSolver->solve(fOdd, left, right, iTolerance, maxStepsCount)) {
            const double u = *root;
            const double E = -V0 + gamma * (u * u) / (a * a);
            levels.push_back({0, E, false});
        }
    }

    // Сортировка по энергии
    std::ranges::sort(levels, [](const EnergyLevel &lhs, const EnergyLevel &rhs) {
        return lhs.energy < rhs.energy;
    });

    for (size_t i = 0; i < levels.size(); ++i) {
        levels[i].index = static_cast<int>(i);
    }

    return levels;
}

std::function<double(double)> SingleWellSolver::getWaveFunction(const EnergyLevel &level,
                                                                double V0,
                                                                double a) const {
    const double gamma = m_gamma;
    const double E = level.energy;

    // Волновые числа: k внутри ямы, kappa снаружи
    const double k = std::sqrt((E + V0) / gamma);
    const double kappa = std::sqrt(-E / gamma);

    // Задаем ненормированную кусочную функцию psi_raw(x)
    std::function<double(double)> rawPsi;
    if (level.isEven) {
        // Чётная: cos(kx) внутри, cos(ka) * exp(-kappa * (|x| - a)) снаружи
        rawPsi = [=](const double x) {
            const double absX = std::abs(x);
            if (absX <= a) {
                return std::cos(k * x);
            }
            return std::cos(k * a) * std::exp(-kappa * (absX - a));
        };
    } else {
        // Нечётная: sin(kx) внутри, sgn(x) * sin(ka) * exp(-kappa * (|x| - a)) снаружи
        rawPsi = [=](const double x) {
            const double absX = std::abs(x);
            if (absX <= a) {
                return std::sin(k * x);
            }
            const double sign = (x >= 0.0) ? 1.0 : -1.0;
            return sign * std::sin(k * a) * std::exp(-kappa * (absX - a));
        };
    }

    // Нормировка: int_{-inf}^{+inf} |psi_raw(x)|^2 dx = inner + tails.
    //
    // 1) Внутри ямы [-a, a] функция гладкая — интегрируем численно.
    //    (Границы ±a не захватываем: там у psi излом, и Симпсон теряет порядок точности.)
    auto psiSquared = [=](const double x) {
        const double p = rawPsi(x);
        return p * p;
    };
    const double inner = m_integrator->calc(-a, a, 400, psiSquared);

    // 2) Хвосты |x| > a — точно и аналитически, без обрезки бесконечного интервала:
    //    |psi|^2 = C^2 * e^{-2*kappa*(|x| - a)},  C = psi_raw(a)
    //    int_a^inf C^2 e^{-2*kappa*(x - a)} dx = C^2 / (2*kappa),
    //    два симметричных хвоста дают C^2 / kappa.
    const double C = rawPsi(a);
    const double tails = C * C / kappa;

    const double normSquare = inner + tails;

    const double normFactor = (normSquare > 0.0) ? (1.0 / std::sqrt(normSquare)) : 1.0;

    // Возвращаем строго отнормированную функцию psi(x)
    return [=](const double x) {
        return normFactor * rawPsi(x);
    };
}
