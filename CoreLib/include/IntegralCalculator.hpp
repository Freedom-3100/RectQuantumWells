//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_INTEGRALCALCULATOR_HPP
#define RECTQUANTUMWELLS_INTEGRALCALCULATOR_HPP
#include <functional>

template<typename T>
class IntegralCalculator {
public:
    virtual ~IntegralCalculator() = default;

    virtual T calc(T left, T right, size_t numberOfPartitions,
                   const std::function<T(T)> &function) const = 0;

    [[nodiscard]] virtual int getOrder() const = 0;
};


template<typename T>
class Simpson final : public IntegralCalculator<T> {
public:
    T calc(T left, T right, size_t numberOfPartitions,
           const std::function<T(T)> &function) const override {
        if (numberOfPartitions % 2 != 0)
            numberOfPartitions++;

        const T simpleStep = (right - left) / numberOfPartitions;
        T result = 0.0;
        result += function(left) + function(right);
        for (size_t i = 1; i < numberOfPartitions; i++) {
            result += i % 2 == 0
                          ? 2.0 * function(left + i * simpleStep)
                          : 4.0 * function(left + i * simpleStep);
        }
        return result * simpleStep / 3.0;
    }

    [[nodiscard]] int getOrder() const override { return 4; }
};


template<typename T>
class RungeMethod final {
public:
    struct RungeInfo {
        size_t stepsCount;
        T integralValue;
        T rungeError;
    };

    static T calc(T left, T right, T eps,
                  const std::function<T(T)> &function,
                  const IntegralCalculator<T> &calculator,
                  const size_t maxSteps = 1048576) {
        size_t curSteps = 2;
        int p = calculator.getOrder();
        T r_pow_p = std::pow(static_cast<T>(2), p);

        T prevI = calculator.calc(left, right, curSteps, function);
        T curI = prevI;
        T error = eps + 1.0;

        while (std::abs(error) > std::abs(eps) && curSteps < maxSteps) {
            curSteps *= 2;
            prevI = curI;
            curI = calculator.calc(left, right, curSteps, function);
            error = std::abs(curI - prevI) / (r_pow_p - 1.0);
        }

        return curI + (curI - prevI) / (r_pow_p - 1.0);
    }
};

#endif //RECTQUANTUMWELLS_INTEGRALCALCULATOR_HPP
