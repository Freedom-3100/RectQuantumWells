//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_PLOTSAMPLING_HPP
#define RECTQUANTUMWELLS_PLOTSAMPLING_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

#include "QuantumSystem.hpp"

namespace PlotSampling {
    /**
     * @brief Сетка точек для отрисовки на отрезке range.
     *
     * Равномерная сетка из samplesCount точек (концы входят строго), дополненная парами точек
     * вплотную слева и справа от каждого разрыва — так ступенька потенциала рисуется вертикальной.
     */
    inline std::vector<double> makeGrid(const SpatialRange range,
                                        const int samplesCount,
                                        const std::vector<double> &discontinuities) {
        constexpr int MIN_SAMPLES = 2;
        if (samplesCount < MIN_SAMPLES || !(range.length() > 0.0)) {
            throw std::invalid_argument("Invalid sampling parameters");
        }

        std::vector<double> grid;
        grid.reserve(static_cast<size_t>(samplesCount) + 2 * discontinuities.size());

        const int lastIndex = samplesCount - 1;
        const double step = range.length() / lastIndex;
        for (int i = 0; i < lastIndex; ++i) {
            grid.push_back(range.left + i * step);
        }
        grid.push_back(range.right);

        constexpr double INFINITY_VALUE = std::numeric_limits<double>::infinity();
        for (const double point: discontinuities) {
            if (point > range.left && point < range.right) {
                grid.push_back(std::nextafter(point, -INFINITY_VALUE));
                grid.push_back(std::nextafter(point, INFINITY_VALUE));
            }
        }

        std::ranges::sort(grid);
        return grid;
    }
}

#endif //RECTQUANTUMWELLS_PLOTSAMPLING_HPP

