//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_SINGLEWELLSOLVER_HPP
#define RECTQUANTUMWELLS_SINGLEWELLSOLVER_HPP
#include <functional>
#include <memory>
#include <vector>

#include "EnergyLevel.hpp"
#include "EqSolver.hpp"
#include "IntegralCalculator.hpp"
#include "Tools.hpp"

class SingleWellSolver {
public:
    explicit SingleWellSolver(
        Constants::UnitSystem unitSystem = Constants::UnitSystem::Nano,
        double effectiveMassRatio = 1.0,
        std::shared_ptr<EqSolver<double> > rootSolver = std::make_shared<DichotomySolver<double> >(),
        std::shared_ptr<IntegralCalculator<double> > integrator = std::make_shared<Simpson<double> >()
    );

    /**
     * @brief Находит все связанные состояния в яме (E < 0).
     * @param V0 Глубина ямы (в единицах выбранной UnitSystem: эВ, Дж, эрг)
     * @param a Полуширина ямы (в единицах выбранной UnitSystem: нм, м, см)
     * @param scanStep Шаг сканирования по безразмерной переменной u (по умолчанию 0.05)
     * @param iTolerance Погрешность вычислений
     * @param maxStepsCount Максимальное число шагов дихотомии
     * @return Список уровней, отсортированный по возрастанию энергии
     */
    [[nodiscard]] std::vector<EnergyLevel> findLevels(double V0,
                                                      double a,
                                                      double scanStep = 0.05,
                                                      double iTolerance = 1e-7,
                                                      size_t maxStepsCount = 100) const;

    /**
     * @brief Возвращает волновую функцию psi(x), нормированную на 1.
     * @param level Уровень энергии
     * @param V0 Глубина ямы
     * @param a Полуширина ямы
     * @return Аналитическая функция psi(x), где x в единицах длины выбранной UnitSystem
     */
    [[nodiscard]] std::function<double(double)> getWaveFunction(const EnergyLevel &level,
                                                                double V0,
                                                                double a) const;

    [[nodiscard]] Constants::UnitSystem getUnitSystem() const { return m_unitSystem; }
    [[nodiscard]] double getGamma() const { return m_gamma; }

private:
    Constants::UnitSystem m_unitSystem;
    double m_effectiveMassRatio;
    double m_gamma; // Предвычисленный hbar^2 / (2 * m) для выбранной системы
    std::shared_ptr<EqSolver<double> > m_rootSolver;
    std::shared_ptr<IntegralCalculator<double> > m_integrator;
};

#endif //RECTQUANTUMWELLS_SINGLEWELLSOLVER_HPP
