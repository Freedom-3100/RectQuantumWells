//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_WELLPLOT_HPP
#define RECTQUANTUMWELLS_WELLPLOT_HPP

#include <QWidget>
#include <vector>

#include "QuantumSystem.hpp"
#include "UnitLabels.hpp"

class QCustomPlot;

/**
 * @brief «Учебниковый» график: потенциал V(x), линии уровней E_n
 * и волновые функции psi_n(x), смещённые на высоту своего уровня.
 *
 * Работает с любой QuantumSystem — про конкретные ямы ничего не знает.
 */
class WellPlot final : public QWidget {
public:
    explicit WellPlot(QWidget *parent = nullptr);

    void display(const QuantumSystem &system, const std::vector<BoundState> &states, const UnitLabels &units);

    void clear();

private:
    void addPotential(const QuantumSystem &system, const std::vector<double> &grid);

    void addStates(const std::vector<BoundState> &states, const std::vector<double> &grid, SpatialRange view);

    QCustomPlot *m_plot;
};

#endif //RECTQUANTUMWELLS_WELLPLOT_HPP

