//
// Created by Mikhail on 06.11.2026.
//
#include <gtest/gtest.h>
#include <EqSolver.hpp>
#include <cmath>

// Тест 1: Простая линейная функция
TEST(DichotomySolverTest, SolvesLinearFunction) {
    DichotomySolver<double> solver;
    // f(x) = 2x - 4, корень в x = 2
    auto root = solver.solve([](double x) { return 2.0 * x - 4.0; }, 0.0, 5.0, 1e-6, 100);

    ASSERT_TRUE(root.has_value());
    EXPECT_NEAR(*root, 2.0, 1e-5);
}

// Тест 2: Отрицательный интервал
TEST(DichotomySolverTest, SolvesOnNegativeInterval) {
    DichotomySolver<double> solver;
    // f(x) = x + 2.5, корень в x = -2.5
    auto root = solver.solve([](double x) { return x + 2.5; }, -4.0, -1.0, 1e-6, 100);

    ASSERT_TRUE(root.has_value());
    EXPECT_NEAR(*root, -2.5, 1e-5);
}

// Тест 3: Нелинейная функция (парабола x^2 - 9 = 0)
TEST(DichotomySolverTest, SolvesQuadraticFunction) {
    DichotomySolver<double> solver;
    auto root = solver.solve([](double x) { return x * x - 9.0; }, 0.0, 5.0, 1e-6, 100);

    ASSERT_TRUE(root.has_value());
    EXPECT_NEAR(*root, 3.0, 1e-5);
}

// Тест 4: Функция без корней должна возвращать std::nullopt
TEST(DichotomySolverTest, ReturnsNulloptWhenNoRoots) {
    DichotomySolver<double> solver;
    // f(x) = x^2 + 1, значений 0 нет
    auto root = solver.solve([](double x) { return x * x + 1.0; }, 1.0, 5.0, 1e-6, 100);

    EXPECT_FALSE(root.has_value());
}

// Тест 5: Попадание точно в границу отрезка
TEST(DichotomySolverTest, RootIsExactlyOnBoundary) {
    DichotomySolver<double> solver;
    // Корень x = 1.0 лежит прямо на левой границе
    auto root = solver.solve([](double x) { return x - 1.0; }, 1.0, 5.0, 1e-6, 100);

    ASSERT_TRUE(root.has_value());
    EXPECT_NEAR(*root, 1.0, 1e-6);
}

// Тест 6: Перепутанные границы (left > right)
TEST(DichotomySolverTest, HandlesSwappedBoundaries) {
    DichotomySolver<double> solver;
    // Передали 5.0 и 0.0 вместо 0.0 и 5.0
    auto root = solver.solve([](double x) { return 2.0 * x - 4.0; }, 5.0, 0.0, 1e-6, 100);

    ASSERT_TRUE(root.has_value());
    EXPECT_NEAR(*root, 2.0, 1e-5);
}
