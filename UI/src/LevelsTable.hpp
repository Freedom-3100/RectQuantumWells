//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_LEVELSTABLE_HPP
#define RECTQUANTUMWELLS_LEVELSTABLE_HPP

#include <QTableWidget>
#include <vector>

#include "QuantumSystem.hpp"
#include "UnitLabels.hpp"

/// Таблица найденных уровней: номер, энергия, чётность
class LevelsTable final : public QTableWidget {
public:
    explicit LevelsTable(QWidget *parent = nullptr);

    void display(const std::vector<BoundState> &states, const UnitLabels &units);
};

#endif //RECTQUANTUMWELLS_LEVELSTABLE_HPP

