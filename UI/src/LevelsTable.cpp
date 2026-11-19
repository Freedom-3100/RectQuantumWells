//
// Created by Mikhail on 06.11.2026.
//

#include "LevelsTable.hpp"

#include <QHeaderView>

#include "UiConfig.hpp"

namespace {
    enum Column {
        INDEX_COLUMN = 0,
        ENERGY_COLUMN,
        PARITY_COLUMN,
        COLUMNS_COUNT
    };

    QTableWidgetItem *makeItem(const QString &text) {
        auto *item = new QTableWidgetItem(text);
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        item->setTextAlignment(Qt::AlignCenter);
        return item;
    }
}

LevelsTable::LevelsTable(QWidget *parent)
    : QTableWidget(0, COLUMNS_COUNT, parent) {
    verticalHeader()->setVisible(false);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setSelectionBehavior(QAbstractItemView::SelectRows);
}

void LevelsTable::display(const std::vector<BoundState> &states, const UnitLabels &units) {
    setHorizontalHeaderLabels({
        QStringLiteral("n"),
        QStringLiteral("E, ") + units.energy,
        QStringLiteral("Чётность"),
    });

    setRowCount(static_cast<int>(states.size()));
    for (int row = 0; row < rowCount(); ++row) {
        const EnergyLevel &level = states[static_cast<size_t>(row)].level;

        setItem(row, INDEX_COLUMN, makeItem(QString::number(level.index)));
        setItem(row, ENERGY_COLUMN, makeItem(QString::number(level.energy, 'g', UiConfig::ENERGY_SIGNIFICANT_DIGITS)));

        auto *parityItem = makeItem(level.isEven ? QStringLiteral("чётное") : QStringLiteral("нечётное"));
        parityItem->setForeground(level.isEven ? UiConfig::EVEN_STATE_COLOR : UiConfig::ODD_STATE_COLOR);
        setItem(row, PARITY_COLUMN, parityItem);
    }
}
