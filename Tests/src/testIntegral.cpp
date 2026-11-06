//
// Created by Mikhail on 06.11.2026.
//

#include <gtest/gtest.h>
#include <IntegralCalculator.hpp>
#include <cmath>
#include <numbers> // C++20 для констант pi и e


TEST(SimpsonTest, ExactForCubicPolynomial) {
    Simpson<double> simpson;
    // int_0^2 (x^3) dx = [x^4 / 4]_0^2 = 16 / 4 = 4.0
    double res = simpson.calc(0.0, 2.0, 2, [](double x) { return x * x * x; });

    EXPECT_NEAR(res, 4.0, 1e-12);
}

// Тест 2: Тригонометрическая функция
TEST(SimpsonTest, IntegratesSine) {
    Simpson<double> simpson;
    // int_0^pi sin(x) dx = [-cos(x)]_0^pi = 2.0
    double res = simpson.calc(0.0, std::numbers::pi, 200, [](double x) {
        return std::sin(x);
    });

    EXPECT_NEAR(res, 2.0, 1e-8);
}

// Тест 3: Экспонента (затухающий хвост, важный для квантовой ямы)
TEST(SimpsonTest, IntegratesExponential) {
    Simpson<double> simpson;
    // int_0^1 e^x dx = e - 1
    double expected = std::numbers::e - 1.0;
    double res = simpson.calc(0.0, 1.0, 100, [](double x) {
        return std::exp(x);
    });

    EXPECT_NEAR(res, expected, 1e-8);
}

// Тест 4: Автоматическая коррекция нечетного числа разбиений
// Если передали 5, алгоритм должен скорректировать до 6 и выдать точный результат
TEST(SimpsonTest, HandlesOddNumberOfPartitions) {
    Simpson<double> simpson;
    // int_0^2 x^2 dx = [x^3 / 3]_0^2 = 8 / 3 ≈ 2.6666667
    double expected = 8.0 / 3.0;
    double res = simpson.calc(0.0, 2.0, 5, [](double x) {
        return x * x;
    });

    EXPECT_NEAR(res, expected, 1e-8);
}

// Тест 5: Отрезок нулевой длины (left == right)
TEST(SimpsonTest, ZeroLengthInterval) {
    Simpson<double> simpson;
    double res = simpson.calc(3.0, 3.0, 20, [](double x) {
        return x * x;
    });

    EXPECT_DOUBLE_EQ(res, 0.0);
}

// Тест 6: Обратный порядок интегрирования (b < a)
// int_b^a f(x) dx = - int_a^b f(x) dx
TEST(SimpsonTest, ReversedLimits) {
    Simpson<double> simpson;
    // int_2^0 x^3 dx = -4.0
    double res = simpson.calc(2.0, 0.0, 2, [](double x) {
        return x * x * x;
    });

    EXPECT_NEAR(res, -4.0, 1e-12);
}


TEST(RungeMethodTest, IntegratesWithAdaptiveAccuracy) {
    Simpson<double> simpson;
    double eps = 1e-8;
    double res = RungeMethod<double>::calc(0.0, std::numbers::pi, eps, [](double x) {
        return std::sin(x);
    }, simpson);
    EXPECT_NEAR(res, 2.0, eps);
}
