//
// Created by Mikhail on 06.11.2026.
//

#include "MainWindow.hpp"

#include <QComboBox>
#include <QLabel>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>
#include <stdexcept>

#include "LevelsTable.hpp"
#include "ParametersPanel.hpp"
#include "UiConfig.hpp"
#include "WellPlot.hpp"

MainWindow::MainWindow(std::vector<std::shared_ptr<const SystemDescriptor>> systems, QWidget *parent)
    : QMainWindow(parent),
      m_systems(std::move(systems)),
      m_unitSystems(availableUnitSystems()),
      m_activeUnitSystem(Constants::UnitSystem::Nano),
      m_systemSelector(new QComboBox),
      m_unitSystemSelector(new QComboBox),
      m_parametersPanel(new ParametersPanel),
      m_levelsTable(new LevelsTable),
      m_plot(new WellPlot) {
    if (m_systems.empty()) {
        throw std::invalid_argument("At least one quantum system must be registered");
    }

    setWindowTitle(QStringLiteral("Прямоугольные квантовые ямы"));
    resize(UiConfig::WINDOW_WIDTH, UiConfig::WINDOW_HEIGHT);

    for (const auto &system: m_systems) {
        m_systemSelector->addItem(system->displayName());
    }

    for (const auto &option: m_unitSystems) {
        m_unitSystemSelector->addItem(option.labels.displayName);
    }
    m_activeUnitSystem = m_unitSystems.front().system;

    // Левая панель: выбор системы, выбор системы единиц, параметры, таблица уровней
    auto *sidePanel = new QWidget;
    auto *sideLayout = new QVBoxLayout(sidePanel);
    sideLayout->addWidget(new QLabel(QStringLiteral("Система")));
    sideLayout->addWidget(m_systemSelector);
    sideLayout->addWidget(new QLabel(QStringLiteral("Система единиц")));
    sideLayout->addWidget(m_unitSystemSelector);
    sideLayout->addWidget(m_parametersPanel);
    sideLayout->addWidget(new QLabel(QStringLiteral("Уровни энергии")));
    sideLayout->addWidget(m_levelsTable, 1);

    auto *splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(sidePanel);
    splitter->addWidget(m_plot);
    splitter->setStretchFactor(1, 1);
    splitter->setSizes({UiConfig::SIDE_PANEL_WIDTH, UiConfig::WINDOW_WIDTH - UiConfig::SIDE_PANEL_WIDTH});
    setCentralWidget(splitter);

    connect(m_systemSelector, &QComboBox::currentIndexChanged, this, &MainWindow::selectSystem);
    connect(m_unitSystemSelector, &QComboBox::currentIndexChanged, this, &MainWindow::selectUnitSystem);
    connect(m_parametersPanel, &ParametersPanel::parametersChanged, this, &MainWindow::recalculate);

    selectSystem(m_systemSelector->currentIndex());
}

void MainWindow::selectSystem(const int index) {
    if (index < 0 || static_cast<size_t>(index) >= m_systems.size()) {
        return;
    }
    m_parametersPanel->setParameters(currentSystem().parameters(currentUnitSystem()));
    recalculate();
}

void MainWindow::selectUnitSystem(const int index) {
    if (index < 0 || static_cast<size_t>(index) >= m_unitSystems.size()) {
        return;
    }

    const Constants::UnitSystem newUnitSystem = currentUnitSystem();
    const ParameterValues oldValues = m_parametersPanel->values();
    ParameterValues convertedValues;

    // Переводим текущие введённые значения из старой системы единиц в новую,
    // чтобы физическая яма оставалась той же самой при переключении единиц
    for (const auto &spec: m_parametersPanel->specs()) {
        if (oldValues.contains(spec.key)) {
            convertedValues.insert(
                spec.key,
                convertParameterValue(oldValues.value(spec.key), spec.dimension, m_activeUnitSystem, newUnitSystem)
            );
        }
    }

    m_activeUnitSystem = newUnitSystem;
    m_parametersPanel->setParameters(currentSystem().parameters(newUnitSystem), convertedValues);
    recalculate();
}

void MainWindow::recalculate() {
    const SystemDescriptor &descriptor = currentSystem();
    const Constants::UnitSystem unitSystem = currentUnitSystem();
    const UnitLabels labels = unitLabels(unitSystem);

    try {
        const auto system = descriptor.create(m_parametersPanel->values(), unitSystem);
        const auto states = system->boundStates();

        m_plot->display(*system, states, labels);
        m_levelsTable->display(states, labels);

        statusBar()->setStyleSheet({});
        statusBar()->showMessage(QStringLiteral("Найдено уровней: %1").arg(states.size()));
    } catch (const std::invalid_argument &error) {
        showError(QString::fromStdString(error.what()));
    }
}

void MainWindow::showError(const QString &message) {
    m_plot->clear();
    m_levelsTable->setRowCount(0);
    statusBar()->setStyleSheet(QStringLiteral("color: %1").arg(UiConfig::ERROR_TEXT_COLOR.name()));
    statusBar()->showMessage(QStringLiteral("Ошибка: ") + message);
}

const SystemDescriptor &MainWindow::currentSystem() const {
    return *m_systems.at(static_cast<size_t>(m_systemSelector->currentIndex()));
}

Constants::UnitSystem MainWindow::currentUnitSystem() const {
    return m_unitSystems.at(static_cast<size_t>(m_unitSystemSelector->currentIndex())).system;
}
