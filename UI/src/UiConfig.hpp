//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_UICONFIG_HPP
#define RECTQUANTUMWELLS_UICONFIG_HPP

#include <QColor>

/// Все визуальные настройки интерфейса собраны здесь — в остальном коде UI нет «магических чисел».
namespace UiConfig {
    // --- Окно ---
    inline constexpr int WINDOW_WIDTH = 1280;
    inline constexpr int WINDOW_HEIGHT = 800;
    inline constexpr int SIDE_PANEL_WIDTH = 380;

    // --- Ввод параметров ---
    // Число делений слайдера на весь диапазон параметра
    inline constexpr int SLIDER_RESOLUTION = 1000;
    // Внутренний запас десятичных знаков QDoubleSpinBox для работы с порядками СИ/СГС (10^-19)
    inline constexpr int SPINBOX_INTERNAL_DECIMALS = 30;

    // --- Дискретизация графиков ---
    inline constexpr int PLOT_SAMPLES_COUNT = 2000;

    // --- Область графика ---
    // Запас по x с каждой стороны, в долях ширины области потенциала
    inline constexpr double X_MARGIN_FRACTION = 0.75;
    // Запас по энергии сверху и снизу, в долях глубины ямы
    inline constexpr double ENERGY_MARGIN_FRACTION = 0.15;

    // --- Волновые функции ---
    // psi_n(x) рисуется со смещением на свой уровень E_n.
    // Максимальная амплитуда — доля от среднего расстояния между соседними уровнями.
    inline constexpr double PSI_AMPLITUDE_FRACTION = 0.45;

    // --- Таблица уровней и оси ---
    inline constexpr int ENERGY_SIGNIFICANT_DIGITS = 6;
    inline constexpr int AXIS_NUMBER_PRECISION = 4;

    // --- Стиль линий ---
    inline constexpr int POTENTIAL_LINE_WIDTH = 3;
    inline constexpr int PSI_LINE_WIDTH = 2;
    inline constexpr int LEVEL_LINE_WIDTH = 1;

    // --- Цвета ---
    inline const QColor POTENTIAL_COLOR{Qt::black};
    inline const QColor EVEN_STATE_COLOR{31, 119, 180}; // синий — чётные (симметричные)
    inline const QColor ODD_STATE_COLOR{214, 39, 40}; // красный — нечётные (антисимметричные)
    inline const QColor LEVEL_LINE_COLOR{150, 150, 150};
    inline const QColor ERROR_TEXT_COLOR{200, 0, 0};
}

#endif //RECTQUANTUMWELLS_UICONFIG_HPP
