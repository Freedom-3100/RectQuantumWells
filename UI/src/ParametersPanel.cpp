//
// Created by Mikhail on 06.11.2026.
//

#include "ParametersPanel.hpp"

#include <QDoubleSpinBox>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLocale>
#include <QSignalBlocker>
#include <QSlider>
#include <algorithm>

#include "UiConfig.hpp"

namespace {
    /**
     * @brief Числовое поле, поддерживающее как обычную ('10.5'), так и экспоненциальную ('1.602e-18') запись.
     *
     * Позволяет комфортно работать во всех четырех системах единиц: Nano, AtomicUnits, CGS и SI.
     */
    class ScientificSpinBox final : public QDoubleSpinBox {
    public:
        explicit ScientificSpinBox(const int significantDigits, QWidget *parent = nullptr)
            : QDoubleSpinBox(parent), m_significantDigits(significantDigits) {
            setDecimals(UiConfig::SPINBOX_INTERNAL_DECIMALS);
            setLocale(QLocale::c());
        }

    protected:
        [[nodiscard]] QString textFromValue(const double value) const override {
            return QString::number(value, 'g', m_significantDigits);
        }

        [[nodiscard]] double valueFromText(const QString &text) const override {
            QString cleaned = text;
            if (!suffix().isEmpty() && cleaned.endsWith(suffix())) {
                cleaned.chop(suffix().size());
            }
            return cleaned.trimmed().toDouble();
        }

        QValidator::State validate(QString &text, int &pos) const override {
            QString copy = text;
            if (!suffix().isEmpty() && copy.endsWith(suffix())) {
                copy.chop(suffix().size());
            }
            copy = copy.trimmed();
            if (copy.isEmpty() || copy == QStringLiteral("-") || copy == QStringLiteral("+") ||
                copy.endsWith(QLatin1Char('e'), Qt::CaseInsensitive) ||
                copy.endsWith(QStringLiteral("e-"), Qt::CaseInsensitive) ||
                copy.endsWith(QStringLiteral("e+"), Qt::CaseInsensitive)) {
                return QValidator::Intermediate;
            }
            bool ok = false;
            const double val = copy.toDouble(&ok);
            if (!ok) {
                return QValidator::Invalid;
            }
            if (val >= minimum() && val <= maximum()) {
                return QValidator::Acceptable;
            }
            return QValidator::Intermediate;
        }

    private:
        int m_significantDigits;
    };

    // Линейное отображение значения параметра на позицию слайдера [0, SLIDER_RESOLUTION] и обратно
    int toSliderPosition(const ParameterSpec &spec, const double value) {
        const double fraction = std::clamp((value - spec.minimum) / (spec.maximum - spec.minimum), 0.0, 1.0);
        return qRound(fraction * UiConfig::SLIDER_RESOLUTION);
    }

    double fromSliderPosition(const ParameterSpec &spec, const int position) {
        const double fraction = static_cast<double>(position) / UiConfig::SLIDER_RESOLUTION;
        return spec.minimum + fraction * (spec.maximum - spec.minimum);
    }
}

ParametersPanel::ParametersPanel(QWidget *parent)
    : QGroupBox(QStringLiteral("Параметры"), parent),
      m_layout(new QFormLayout(this)) {
}

void ParametersPanel::setParameters(const std::vector<ParameterSpec> &specs,
                                    const ParameterValues &initialValues) {
    while (m_layout->rowCount() > 0) {
        m_layout->removeRow(0);
    }
    m_inputs.clear();
    m_specs = specs;

    for (const auto &spec: specs) {
        const double value = initialValues.contains(spec.key)
                                 ? std::clamp(initialValues.value(spec.key), spec.minimum, spec.maximum)
                                 : spec.defaultValue;
        addInput(spec, value);
    }
}

ParameterValues ParametersPanel::values() const {
    ParameterValues result;
    for (const auto &input: m_inputs) {
        result.insert(input.spec.key, input.spinBox->value());
    }
    return result;
}

void ParametersPanel::addInput(const ParameterSpec &spec, const double initialValue) {
    auto *spinBox = new ScientificSpinBox(spec.significantDigits);
    spinBox->setRange(spec.minimum, spec.maximum);
    spinBox->setSingleStep(spec.step);
    spinBox->setSuffix(QStringLiteral(" ") + spec.unit);
    spinBox->setValue(initialValue);

    auto *slider = new QSlider(Qt::Horizontal);
    slider->setRange(0, UiConfig::SLIDER_RESOLUTION);
    slider->setValue(toSliderPosition(spec, initialValue));

    // Поле -> слайдер (без обратной связи) и уведомление об изменении
    connect(spinBox, &QDoubleSpinBox::valueChanged, this, [this, spec, slider](const double value) {
        const QSignalBlocker blocker(slider);
        slider->setValue(toSliderPosition(spec, value));
        emit parametersChanged();
    });
    // Слайдер -> поле (поле само сообщит об изменении)
    connect(slider, &QSlider::valueChanged, spinBox, [spec, spinBox](const int position) {
        spinBox->setValue(fromSliderPosition(spec, position));
    });

    auto *row = new QWidget;
    auto *rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(0, 0, 0, 0);
    rowLayout->addWidget(slider, 1);
    rowLayout->addWidget(spinBox);

    m_layout->addRow(spec.label, row);
    m_inputs.push_back({spec, spinBox, slider});
}
