//
// Created by Mikhail on 06.11.2026.
//

#include "SingleWellDescriptor.hpp"

#include <stdexcept>

#include "SingleWellSystem.hpp"

namespace {
    const QString DEPTH_KEY = QStringLiteral("depth");
    const QString HALF_WIDTH_KEY = QStringLiteral("halfWidth");

    // Базовые диапазоны ввода и значения по умолчанию заданы в системе Nano (эВ, нм)
    // и автоматически переводятся в выбранную пользователем UnitSystem
    constexpr auto BASE_SYSTEM = Constants::UnitSystem::Nano;

    constexpr double DEPTH_MIN_EV = 0.01;
    constexpr double DEPTH_MAX_EV = 50.0;
    constexpr double DEPTH_DEFAULT_EV = 10.0;
    constexpr double DEPTH_STEP_EV = 0.1;

    constexpr double HALF_WIDTH_MIN_NM = 0.01;
    constexpr double HALF_WIDTH_MAX_NM = 5.0;
    constexpr double HALF_WIDTH_DEFAULT_NM = 1.0;
    constexpr double HALF_WIDTH_STEP_NM = 0.01;

    constexpr int INPUT_SIGNIFICANT_DIGITS = 4;

    double requireValue(const ParameterValues &values, const QString &key) {
        const auto it = values.constFind(key);
        if (it == values.cend()) {
            throw std::invalid_argument("Missing parameter: " + key.toStdString());
        }
        return it.value();
    }
}

QString SingleWellDescriptor::displayName() const {
    return QStringLiteral("Одиночная яма");
}

std::vector<ParameterSpec> SingleWellDescriptor::parameters(const Constants::UnitSystem unitSystem) const {
    const UnitLabels labels = unitLabels(unitSystem);
    return {
        {
            DEPTH_KEY,
            QStringLiteral("Глубина V₀"),
            labels.energy,
            ParameterDimension::Energy,
            Constants::Convert::energy(DEPTH_MIN_EV, BASE_SYSTEM, unitSystem),
            Constants::Convert::energy(DEPTH_MAX_EV, BASE_SYSTEM, unitSystem),
            Constants::Convert::energy(DEPTH_DEFAULT_EV, BASE_SYSTEM, unitSystem),
            Constants::Convert::energy(DEPTH_STEP_EV, BASE_SYSTEM, unitSystem),
            INPUT_SIGNIFICANT_DIGITS
        },
        {
            HALF_WIDTH_KEY,
            QStringLiteral("Полуширина a"),
            labels.length,
            ParameterDimension::Length,
            Constants::Convert::length(HALF_WIDTH_MIN_NM, BASE_SYSTEM, unitSystem),
            Constants::Convert::length(HALF_WIDTH_MAX_NM, BASE_SYSTEM, unitSystem),
            Constants::Convert::length(HALF_WIDTH_DEFAULT_NM, BASE_SYSTEM, unitSystem),
            Constants::Convert::length(HALF_WIDTH_STEP_NM, BASE_SYSTEM, unitSystem),
            INPUT_SIGNIFICANT_DIGITS
        },
    };
}

std::unique_ptr<QuantumSystem> SingleWellDescriptor::create(const ParameterValues &values,
                                                            const Constants::UnitSystem unitSystem) const {
    const SingleWellParameters parameters{
        .depth = requireValue(values, DEPTH_KEY),
        .halfWidth = requireValue(values, HALF_WIDTH_KEY),
    };
    return std::make_unique<SingleWellSystem>(parameters, std::make_shared<const SingleWellSolver>(unitSystem));
}
