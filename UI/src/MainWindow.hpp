//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_MAINWINDOW_HPP
#define RECTQUANTUMWELLS_MAINWINDOW_HPP

#include <QMainWindow>
#include <memory>
#include <vector>

#include "SystemDescriptor.hpp"

class QComboBox;
class LevelsTable;
class ParametersPanel;
class WellPlot;

/**
 * @brief Главное окно — контроллер.
 *
 * Связывает выбор системы, выбор системы единиц (Nano, Atomic, CGS, SI),
 * панель параметров, график и таблицу.
 */
class MainWindow final : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::vector<std::shared_ptr<const SystemDescriptor>> systems, QWidget *parent = nullptr);

private:
    void selectSystem(int index);

    void selectUnitSystem(int index);

    void recalculate();

    void showError(const QString &message);

    [[nodiscard]] const SystemDescriptor &currentSystem() const;

    [[nodiscard]] Constants::UnitSystem currentUnitSystem() const;

    std::vector<std::shared_ptr<const SystemDescriptor>> m_systems;
    std::vector<UnitSystemOption> m_unitSystems;
    Constants::UnitSystem m_activeUnitSystem;

    QComboBox *m_systemSelector;
    QComboBox *m_unitSystemSelector;
    ParametersPanel *m_parametersPanel;
    LevelsTable *m_levelsTable;
    WellPlot *m_plot;
};

#endif //RECTQUANTUMWELLS_MAINWINDOW_HPP
