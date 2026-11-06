//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_EQSOLVER_HPP
#define RECTQUANTUMWELLS_EQSOLVER_HPP

#include <optional>
#include <functional>
#include <cmath>
#include <concepts>

template<typename T>
    requires std::floating_point<T>
class EqSolver {
public:
    virtual ~EqSolver() = default;

    virtual std::optional<T> solve(std::function<T(T)> iFunc, T leftBoundary, T rightBoundary, T iTolerance,
                                   size_t maxStepsCount) = 0;
};

template<typename T>
    requires std::floating_point<T>
class DichotomySolver final : public EqSolver<T> {
public:
    std::optional<T> solve(std::function<T(T)> iFunc, T leftBoundary, T rightBoundary, T iTolerance,
                           size_t maxStepsCount) override {
        if (leftBoundary > rightBoundary)
            std::swap(leftBoundary, rightBoundary);

        auto fLeft = iFunc(leftBoundary);
        auto fRight = iFunc(rightBoundary);
        if (fLeft * fRight > 0.0)
            return std::nullopt;

        auto nextStepVal = leftBoundary;
        auto accuracy = (rightBoundary - leftBoundary) / 2.0;
        size_t stepsCount = 0;
        while (accuracy > iTolerance && stepsCount < maxStepsCount) {
            nextStepVal = (rightBoundary + leftBoundary) / 2;
            auto nextFuncRes = iFunc(nextStepVal);

            if (std::abs(nextFuncRes) < iTolerance)
                return nextStepVal;

            if (fLeft * nextFuncRes > 0.0) {
                leftBoundary = nextStepVal;
                fLeft = nextFuncRes;
            } else {
                rightBoundary = nextStepVal;
            }
            accuracy = (rightBoundary - leftBoundary) / 2.0;
            stepsCount++;
        }
        if (stepsCount >= maxStepsCount)
            return std::nullopt;

        return (leftBoundary + rightBoundary) / 2.0;
    }
};

#endif //RECTQUANTUMWELLS_EQSOLVER_HPP
