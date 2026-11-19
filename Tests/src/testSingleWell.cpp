//
// Created by Mikhail on 06.11.2026.
//
#include <gtest/gtest.h>
#include <SingleWellSolver.hpp>
#include <SingleWellSystem.hpp>
#include <cmath>
#include <numbers>

namespace {
    // Эталонная нормировка: интеграл разбит по границам ямы, очень мелкая сетка
    double referenceNorm(const std::function<double(double)> &psi, double a, double kappa) {
        Simpson<double> simpson;
        auto sq = [&](double x) {
            const double p = psi(x);
            return p * p;
        };
        const double tail = a + 40.0 / kappa;
        return simpson.calc(-a, a, 20000, sq) + 2.0 * simpson.calc(a, tail, 200000, sq);
    }

    int expectedLevelsCount(double V0, double a, double gamma) {
        const double R = std::sqrt(V0 * a * a / gamma);
        return static_cast<int>(std::ceil(2.0 * R / std::numbers::pi));
    }
}

// Число уровней в прямоугольной яме: N = ceil(2R / pi)
TEST(SingleWellSolverTest, LevelsCountMatchesTheory) {
    SingleWellSolver solver;
    const double V0 = 10.0, a = 1.0;
    const auto levels = solver.findLevels(V0, a);

    EXPECT_EQ(static_cast<int>(levels.size()), expectedLevelsCount(V0, a, solver.getGamma()));
}

// В одномерной яме всегда есть хотя бы один (чётный) уровень
TEST(SingleWellSolverTest, AlwaysHasGroundState) {
    SingleWellSolver solver;
    const auto levels = solver.findLevels(10.0, 0.05);

    ASSERT_EQ(levels.size(), 1u);
    EXPECT_TRUE(levels[0].isEven);
    EXPECT_LT(levels[0].energy, 0.0);
    EXPECT_GT(levels[0].energy, -10.0);
}

// Уровни отсортированы, лежат в (-V0, 0), чётность чередуется, начиная с чётного
TEST(SingleWellSolverTest, LevelsAreSortedAndAlternateParity) {
    SingleWellSolver solver;
    const double V0 = 10.0;
    const auto levels = solver.findLevels(V0, 1.0);
    ASSERT_FALSE(levels.empty());

    for (size_t i = 0; i < levels.size(); ++i) {
        EXPECT_EQ(levels[i].index, static_cast<int>(i));
        EXPECT_GT(levels[i].energy, -V0);
        EXPECT_LT(levels[i].energy, 0.0);
        EXPECT_EQ(levels[i].isEven, i % 2 == 0);
        if (i > 0) {
            EXPECT_GT(levels[i].energy, levels[i - 1].energy);
        }
    }
}

// «Выталкивание» уровней: при сужении ямы уровней становится меньше, а основной уровень поднимается
TEST(SingleWellSolverTest, NarrowingPushesLevelsOut) {
    SingleWellSolver solver;
    const double V0 = 10.0;
    const auto wide = solver.findLevels(V0, 1.0);
    const auto narrow = solver.findLevels(V0, 0.3);

    ASSERT_FALSE(wide.empty());
    ASSERT_FALSE(narrow.empty());
    EXPECT_LT(narrow.size(), wide.size());
    EXPECT_GT(narrow[0].energy, wide[0].energy);
}

// Нормировка всех уровней, включая верхний
TEST(SingleWellSolverTest, WaveFunctionsAreNormalized) {
    SingleWellSolver solver;
    const double V0 = 10.0, a = 1.0;

    for (const auto &level: solver.findLevels(V0, a)) {
        const auto psi = solver.getWaveFunction(level, V0, a);
        const double kappa = std::sqrt(-level.energy / solver.getGamma());
        EXPECT_NEAR(referenceNorm(psi, a, kappa), 1.0, 1e-4) << "level " << level.index;
    }
}

// Регрессия: уровень у самого края (E -> 0, kappa -> 0) с очень длинным хвостом
TEST(SingleWellSolverTest, WeaklyBoundLevelIsNormalized) {
    SingleWellSolver solver;
    const double a = 1.0;
    const double u = std::numbers::pi / 2.0 + 1e-3; // R чуть больше pi/2 -> второй уровень почти у нуля
    const double V0 = solver.getGamma() * u * u / (a * a);

    const auto levels = solver.findLevels(V0, a);
    ASSERT_EQ(levels.size(), 2u);

    const auto &top = levels.back();
    const auto psi = solver.getWaveFunction(top, V0, a);
    const double kappa = std::sqrt(-top.energy / solver.getGamma());
    EXPECT_NEAR(referenceNorm(psi, a, kappa), 1.0, 1e-4);
}

// Симметрия: psi(-x) = psi(x) для чётных и psi(-x) = -psi(x) для нечётных
TEST(SingleWellSolverTest, WaveFunctionParity) {
    SingleWellSolver solver;
    const double V0 = 10.0, a = 1.0;

    for (const auto &level: solver.findLevels(V0, a)) {
        const auto psi = solver.getWaveFunction(level, V0, a);
        const double sign = level.isEven ? 1.0 : -1.0;
        for (const double x: {0.1, 0.5, 0.99, 1.5, 3.0}) {
            EXPECT_NEAR(psi(-x), sign * psi(x), 1e-12);
        }
    }
}

// Непрерывность psi на стенке ямы x = a
TEST(SingleWellSolverTest, WaveFunctionIsContinuousAtWall) {
    SingleWellSolver solver;
    const double V0 = 10.0, a = 1.0;

    for (const auto &level: solver.findLevels(V0, a)) {
        const auto psi = solver.getWaveFunction(level, V0, a);
        EXPECT_NEAR(psi(a - 1e-9), psi(a + 1e-9), 1e-6);
    }
}

// Результат не зависит от системы единиц: та же яма в СИ даёт ту же энергию
TEST(SingleWellSolverTest, UnitSystemIndependence) {
    SingleWellSolver nano(Constants::UnitSystem::Nano);
    SingleWellSolver si(Constants::UnitSystem::SI);

    const auto levelsNano = nano.findLevels(10.0, 1.0);
    const auto levelsSi = si.findLevels(Constants::Convert::eV_to_J(10.0), 1e-9);

    ASSERT_EQ(levelsNano.size(), levelsSi.size());
    for (size_t i = 0; i < levelsNano.size(); ++i) {
        EXPECT_NEAR(Constants::Convert::J_to_eV(levelsSi[i].energy), levelsNano[i].energy, 1e-4);
    }
}

// Некорректные параметры
TEST(SingleWellSolverTest, InvalidInputs) {
    SingleWellSolver solver;
    EXPECT_TRUE(solver.findLevels(-1.0, 1.0).empty());
    EXPECT_TRUE(solver.findLevels(10.0, 0.0).empty());
    EXPECT_THROW(SingleWellSolver(Constants::UnitSystem::Nano, 0.0), std::invalid_argument);
    EXPECT_THROW(SingleWellSolver(Constants::UnitSystem::Nano, 1.0, nullptr), std::invalid_argument);
}

// Полиморфный интерфейс QuantumSystem / SingleWellSystem
TEST(SingleWellSystemTest,PolymorphicInterfaceAndPotential) {
    const std::unique_ptr<QuantumSystem> system = std::make_unique<SingleWellSystem>(
        SingleWellParameters{.depth = 10.0, .halfWidth = 1.0}
    );

    EXPECT_DOUBLE_EQ(system->potential(0.0), -10.0);
    EXPECT_DOUBLE_EQ(system->potential(1.0), -10.0);
    EXPECT_DOUBLE_EQ(system->potential(1.5), 0.0);
    EXPECT_DOUBLE_EQ(system->potentialMinimum(), -10.0);

    const auto support = system->potentialSupport();
    EXPECT_DOUBLE_EQ(support.left, -1.0);
    EXPECT_DOUBLE_EQ(support.right, 1.0);

    const auto discontinuities = system->potentialDiscontinuities();
    ASSERT_EQ(discontinuities.size(), 2u);
    EXPECT_DOUBLE_EQ(discontinuities[0], -1.0);
    EXPECT_DOUBLE_EQ(discontinuities[1], 1.0);

    const auto states = system->boundStates();
    ASSERT_EQ(states.size(), 11u);
    EXPECT_TRUE(states.front().level.isEven);
    EXPECT_GT(std::abs(states.front().psi(0.0)), 0.0);
}

TEST(SingleWellSystemTest, RejectsInvalidParameters) {
    EXPECT_THROW(SingleWellSystem({.depth = -1.0, .halfWidth = 1.0}), std::invalid_argument);
    EXPECT_THROW(SingleWellSystem({.depth = 10.0, .halfWidth = 0.0}), std::invalid_argument);
    EXPECT_THROW(SingleWellSystem({.depth = 10.0, .halfWidth = 1.0}, nullptr), std::invalid_argument);
}


