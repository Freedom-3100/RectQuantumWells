//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_QUANTUMSYSTEM_HPP
#define RECTQUANTUMWELLS_QUANTUMSYSTEM_HPP

#include <functional>
#include <vector>

#include "EnergyLevel.hpp"

/// Связанное состояние: уровень энергии и его нормированная волновая функция psi(x)
struct BoundState {
    EnergyLevel level;
    std::function<double(double)> psi;
};

/// Отрезок координат [left, right]
struct SpatialRange {
    double left;
    double right;

    [[nodiscard]] double length() const { return right - left; }
};

/**
 * @brief Абстрактная одномерная квантовая система с потенциалом V(x).
 *
 * Соглашения:
 *  - V(x) = 0 вне области potentialSupport(), внутри V(x) <= 0;
 *  - все величины выражены в единицах UnitSystem, выбранной при создании конкретной системы.
 *
 * Реализации: одиночная прямоугольная яма, двойная яма и т.д.
 * Интерфейс ничего не знает об UI — по нему можно и рисовать, и тестировать.
 */
class QuantumSystem {
public:
    virtual ~QuantumSystem() = default;

    /// Потенциальная энергия V(x)
    [[nodiscard]] virtual double potential(double x) const = 0;

    /// Точки разрыва V(x) (стенки ям), по возрастанию
    [[nodiscard]] virtual std::vector<double> potentialDiscontinuities() const = 0;

    /// Область, вне которой V(x) = 0
    [[nodiscard]] virtual SpatialRange potentialSupport() const = 0;

    /// Минимальное значение потенциала (дно ямы), например -V0
    [[nodiscard]] virtual double potentialMinimum() const = 0;

    /// Все связанные состояния (E < 0), отсортированные по возрастанию энергии
    [[nodiscard]] virtual std::vector<BoundState> boundStates() const = 0;
};

#endif //RECTQUANTUMWELLS_QUANTUMSYSTEM_HPP

