//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_UNITLABELS_HPP
#define RECTQUANTUMWELLS_UNITLABELS_HPP

#include <QString>
#include <vector>

#include "Tools.hpp"

/// Подписи единиц измерения для выбранной системы единиц ядра
struct UnitLabels {
    QString displayName; // Полное имя системы для выпадающего списка
    QString energy; // Единица энергии
    QString length; // Единица длины
};

struct UnitSystemOption {
    Constants::UnitSystem system;
    UnitLabels labels;
};

inline UnitLabels unitLabels(const Constants::UnitSystem system) {
    switch (system) {
        case Constants::UnitSystem::Nano:
            return {QStringLiteral("Нано (эВ, нм)"), QStringLiteral("эВ"), QStringLiteral("нм")};
        case Constants::UnitSystem::AtomicUnits:
            return {QStringLiteral("Атомная (Ha, a₀)"), QStringLiteral("Ha"), QStringLiteral("a₀")};
        case Constants::UnitSystem::CGS:
            return {QStringLiteral("СГС (эрг, см)"), QStringLiteral("эрг"), QStringLiteral("см")};
        case Constants::UnitSystem::SI:
            return {QStringLiteral("СИ (Дж, м)"), QStringLiteral("Дж"), QStringLiteral("м")};
    }
    return {};
}

inline std::vector<UnitSystemOption> availableUnitSystems() {
    return {
        {Constants::UnitSystem::Nano, unitLabels(Constants::UnitSystem::Nano)},
        {Constants::UnitSystem::AtomicUnits, unitLabels(Constants::UnitSystem::AtomicUnits)},
        {Constants::UnitSystem::CGS, unitLabels(Constants::UnitSystem::CGS)},
        {Constants::UnitSystem::SI, unitLabels(Constants::UnitSystem::SI)},
    };
}

#endif //RECTQUANTUMWELLS_UNITLABELS_HPP
