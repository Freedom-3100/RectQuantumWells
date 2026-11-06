//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_ROOTBRACKETFINDER_HPP
#define RECTQUANTUMWELLS_ROOTBRACKETFINDER_HPP
#include <concepts>
#include <functional>
#include <vector>

template<typename T>
    requires std::floating_point<T>
class RootBracketFinder {
public:
    struct Interval {
        T left;
        T right;
    };

    static std::vector<Interval> findIntervals(const std::function<T(T)> &func, T left, T right, T requestedStep) {
        if (left > right)
            std::swap(left, right);

        if (left == right || requestedStep <= 0.0)
            return {};

        auto stepsCount = static_cast<size_t>(std::ceil((right - left) / requestedStep));
        if (stepsCount == 0)
            stepsCount = 1;

        T actualStep = (right - left) / stepsCount;
        std::vector<Interval> intervals;

        T xPrev = left;
        T fPrev = func(xPrev);
        for (size_t i = 1; i <= stepsCount; ++i) {
            T xCurr = (i == stepsCount) ? right : left + static_cast<T>(i) * actualStep;
            T fCurr = func(xCurr);

            if (fPrev * fCurr < static_cast<T>(0) || fCurr == static_cast<T>(0)) {
                intervals.push_back({xPrev, xCurr});
            } else if (i == 1 && fPrev == static_cast<T>(0)) {
                intervals.push_back({xPrev, xCurr});
            }

            xPrev = xCurr;
            fPrev = fCurr;
        }
        return intervals;
    }
};

#endif //RECTQUANTUMWELLS_ROOTBRACKETFINDER_HPP
