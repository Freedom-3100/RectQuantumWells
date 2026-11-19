//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_PARAMETERSPANEL_HPP
#define RECTQUANTUMWELLS_PARAMETERSPANEL_HPP

#include <QGroupBox>
#include <vector>

#include "SystemDescriptor.hpp"

class QDoubleSpinBox;
class QFormLayout;
class QSlider;

/**
 * @brief Панель ввода параметров.
 *
 * Сама строит поля (слайдер + числовое поле с поддержкой экспоненциальной записи для СИ/СГС)
 * по списку ParameterSpec, поэтому не зависит от того, какая система или UnitSystem выбраны.
 */
class ParametersPanel final : public QGroupBox {
    Q_OBJECT

public:
    explicit ParametersPanel(QWidget *parent = nullptr);

    /**
     * @brief Перестраивает поля ввода под новый набор параметров.
     * @param specs Описания параметров
     * @param initialValues Начальные значения (если ключ отсутствует, берётся spec.defaultValue)
     */
    void setParameters(const std::vector<ParameterSpec> &specs, const ParameterValues &initialValues = {});

    [[nodiscard]] ParameterValues values() const;

    [[nodiscard]] const std::vector<ParameterSpec> &specs() const { return m_specs; }

signals:
    /// Любой параметр изменился
    void parametersChanged();

private:
    struct Input {
        ParameterSpec spec;
        QDoubleSpinBox *spinBox;
        QSlider *slider;
    };

    void addInput(const ParameterSpec &spec, double initialValue);

    QFormLayout *m_layout;
    std::vector<ParameterSpec> m_specs;
    std::vector<Input> m_inputs;
};

#endif //RECTQUANTUMWELLS_PARAMETERSPANEL_HPP
