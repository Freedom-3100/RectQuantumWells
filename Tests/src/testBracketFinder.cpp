//
// Created by Mikhail on 06.11.2026.
//
#include <gtest/gtest.h>
#include <RootBracketFinder.hpp>
#include <EqSolver.hpp>
#include <cmath>

// Тест 1: Поиск нескольких корней (полином 3-й степени с корнями 1.0, 2.0, 3.0)
TEST(RootBracketFinderTest, FindsMultipleRoots) {
    auto poly = [](double x) { return (x - 1.0) * (x - 2.0) * (x - 3.0); };

    // Шагаем по [0.0, 4.0] с шагом 0.35
    auto intervals = RootBracketFinder<double>::findIntervals(poly, 0.0, 4.0, 0.35);

    ASSERT_EQ(intervals.size(), 3);

    // Корень 1.0 лежит в первом интервале
    EXPECT_LE(intervals[0].left, 1.0);
    EXPECT_GE(intervals[0].right, 1.0);

    // Корень 2.0 лежит во втором интервале
    EXPECT_LE(intervals[1].left, 2.0);
    EXPECT_GE(intervals[1].right, 2.0);

    // Корень 3.0 лежит в третьем интервале
    EXPECT_LE(intervals[2].left, 3.0);
    EXPECT_GE(intervals[2].right, 3.0);
}

// Тест 2: Гарантия строгого включения границы right (даже если шаг не делит отрезок нацело)
TEST(RootBracketFinderTest, GuaranteesRightBoundaryIncluded) {
    // Отрезок [0.0, 5.0], шаг 1.3 (5.0 / 1.3 ≈ 3.84 -> будет 4 шага)
    // Функция меняет знак только у самого правого края (корень в 4.9)
    auto func = [](double x) { return x - 4.9; };
    auto intervals = RootBracketFinder<double>::findIntervals(func, 0.0, 5.0, 1.3);

    ASSERT_FALSE(intervals.empty());
    // Правая граница последнего интервала должна строго равняться 5.0
    EXPECT_DOUBLE_EQ(intervals.back().right, 5.0);
}

// Тест 3: Функция без корней (всегда положительна)
TEST(RootBracketFinderTest, ReturnsEmptyWhenNoRoots) {
    auto func = [](double x) { return x * x + 10.0; };
    auto intervals = RootBracketFinder<double>::findIntervals(func, -5.0, 5.0, 0.5);

    EXPECT_TRUE(intervals.empty());
}

// Тест 4: Некорректный шаг (шаг <= 0 или пустой интервал)
TEST(RootBracketFinderTest, HandlesInvalidInputs) {
    auto func = [](double x) { return x; };

    EXPECT_TRUE(RootBracketFinder<double>::findIntervals(func, 0.0, 5.0, 0.0).empty());
    EXPECT_TRUE(RootBracketFinder<double>::findIntervals(func, 0.0, 5.0, -1.0).empty());
    EXPECT_TRUE(RootBracketFinder<double>::findIntervals(func, 2.0, 2.0, 0.5).empty());
}

// Тест 5: Перепутанные границы (left > right)
TEST(RootBracketFinderTest, HandlesSwappedBoundaries) {
    auto func = [](double x) { return x - 2.5; };
    // Передаем 5.0 и 0.0 вместо 0.0 и 5.0
    auto intervals = RootBracketFinder<double>::findIntervals(func, 5.0, 0.0, 0.5);

    ASSERT_EQ(intervals.size(), 1);
    EXPECT_LE(intervals[0].left, 2.5);
    EXPECT_GE(intervals[0].right, 2.5);
}

// Тест 6: Главный тест связки — RootBracketFinder + DichotomySolver (находит ВСЕ корни)
TEST(RootBracketFinderTest, EndToEndWithDichotomySolver) {
    auto poly = [](double x) { return (x - 1.0) * (x - 2.0) * (x - 3.0); };

    // 1. Находим интервалы
    auto intervals = RootBracketFinder<double>::findIntervals(poly, 0.0, 4.0, 0.4);
    ASSERT_EQ(intervals.size(), 3);

    // 2. Уточняем корни через дихотомию
    DichotomySolver<double> solver;
    std::vector<double> calculatedRoots;

    for (const auto &interval: intervals) {
        auto root = solver.solve(poly, interval.left, interval.right, 1e-6, 100);
        ASSERT_TRUE(root.has_value());
        calculatedRoots.push_back(*root);
    }

    // Проверяем точность каждого найденного корня
    EXPECT_NEAR(calculatedRoots[0], 1.0, 1e-5);
    EXPECT_NEAR(calculatedRoots[1], 2.0, 1e-5);
    EXPECT_NEAR(calculatedRoots[2], 3.0, 1e-5);
}