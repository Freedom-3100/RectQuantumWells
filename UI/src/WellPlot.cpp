//
// Created by Mikhail on 06.11.2026.
//

#include "WellPlot.hpp"

#include <QVBoxLayout>
#include <algorithm>
#include <cmath>

#include "PlotSampling.hpp"
#include "UiConfig.hpp"
#include "qcustomplot.h"

namespace {
    QColor stateColor(const EnergyLevel &level) {
        return level.isEven ? UiConfig::EVEN_STATE_COLOR : UiConfig::ODD_STATE_COLOR;
    }

    /// Среднее расстояние между соседними уровнями (для одного уровня — расстояние до границы E = 0)
    double meanLevelSpacing(const std::vector<BoundState> &states) {
        if (states.size() > 1) {
            const double spread = states.back().level.energy - states.front().level.energy;
            return spread / static_cast<double>(states.size() - 1);
        }
        return std::abs(states.front().level.energy);
    }

    /// Максимум |psi| по всем состояниям на сетке
    double maxAbsPsi(const std::vector<BoundState> &states, const std::vector<double> &grid) {
        double result = 0.0;
        for (const auto &state: states) {
            for (const double x: grid) {
                result = std::max(result, std::abs(state.psi(x)));
            }
        }
        return result;
    }
}

WellPlot::WellPlot(QWidget *parent)
    : QWidget(parent), m_plot(new QCustomPlot(this)) {
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_plot);

    m_plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    // Формат "gb" автоматически выводит обычные числа для Нано/Атомных и красивые степени 10^-18 для СИ/СГС
    m_plot->xAxis->setNumberFormat(QStringLiteral("gb"));
    m_plot->xAxis->setNumberPrecision(UiConfig::AXIS_NUMBER_PRECISION);
    m_plot->yAxis->setNumberFormat(QStringLiteral("gb"));
    m_plot->yAxis->setNumberPrecision(UiConfig::AXIS_NUMBER_PRECISION);
}

void WellPlot::clear() {
    m_plot->clearGraphs();
    m_plot->replot();
}

void WellPlot::display(const QuantumSystem &system, const std::vector<BoundState> &states, const UnitLabels &units) {
    m_plot->clearGraphs();

    // Видимая область по x: область потенциала с запасом по краям, чтобы были видны хвосты psi
    const SpatialRange support = system.potentialSupport();
    const double xMargin = support.length() * UiConfig::X_MARGIN_FRACTION;
    const SpatialRange view{support.left - xMargin, support.right + xMargin};

    const auto grid = PlotSampling::makeGrid(view, UiConfig::PLOT_SAMPLES_COUNT,
                                             system.potentialDiscontinuities());

    addPotential(system, grid);
    addStates(states, grid, view);

    // Видимая область по энергии: от дна ямы до E = 0 с запасом
    const double depth = -system.potentialMinimum();
    const double energyMargin = depth * UiConfig::ENERGY_MARGIN_FRACTION;

    m_plot->xAxis->setLabel(QStringLiteral("x, ") + units.length);
    m_plot->yAxis->setLabel(QStringLiteral("E, ") + units.energy);
    m_plot->xAxis->setRange(view.left, view.right);
    m_plot->yAxis->setRange(system.potentialMinimum() - energyMargin, energyMargin);
    m_plot->replot();
}

void WellPlot::addPotential(const QuantumSystem &system, const std::vector<double> &grid) {
    QVector<double> xs;
    QVector<double> vs;
    xs.reserve(static_cast<qsizetype>(grid.size()));
    vs.reserve(static_cast<qsizetype>(grid.size()));
    for (const double x: grid) {
        xs.push_back(x);
        vs.push_back(system.potential(x));
    }

    QCPGraph *graph = m_plot->addGraph();
    graph->setData(xs, vs, true);
    graph->setPen(QPen(UiConfig::POTENTIAL_COLOR, UiConfig::POTENTIAL_LINE_WIDTH));
}

void WellPlot::addStates(const std::vector<BoundState> &states, const std::vector<double> &grid,
                         const SpatialRange view) {
    if (states.empty()) {
        return;
    }

    // Общий масштаб для всех psi: сохраняет их относительные амплитуды
    const double maxPsi = maxAbsPsi(states, grid);
    if (!(maxPsi > 0.0)) {
        return;
    }
    const double psiScale = UiConfig::PSI_AMPLITUDE_FRACTION * meanLevelSpacing(states) / maxPsi;

    QVector<double> xs;
    xs.reserve(static_cast<qsizetype>(grid.size()));
    for (const double x: grid) {
        xs.push_back(x);
    }

    for (const auto &state: states) {
        const double energy = state.level.energy;

        // Горизонтальная линия уровня
        QCPGraph *levelLine = m_plot->addGraph();
        levelLine->setData({view.left, view.right}, {energy, energy}, true);
        levelLine->setPen(QPen(UiConfig::LEVEL_LINE_COLOR, UiConfig::LEVEL_LINE_WIDTH, Qt::DashLine));

        // psi_n(x), смещённая на E_n
        QVector<double> ys;
        ys.reserve(xs.size());
        for (const double x: grid) {
            ys.push_back(energy + psiScale * state.psi(x));
        }
        QCPGraph *psiGraph = m_plot->addGraph();
        psiGraph->setData(xs, ys, true);
        psiGraph->setPen(QPen(stateColor(state.level), UiConfig::PSI_LINE_WIDTH));
    }
}
