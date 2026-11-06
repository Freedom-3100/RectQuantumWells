//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_SYSTEMDESCRIPTOR_HPP
#define RECTQUANTUMWELLS_SYSTEMDESCRIPTOR_HPP

#include <QMap>
#include <QString>
#include <memory>
#include <vector>

#include "QuantumSystem.hpp"
#include "UnitLabels.hpp"

/// Физическая размерность параметра — позволяет автоматически переводить введённые значения при смене UnitSystem
enum class ParameterDimension {
    Energy,
    Length,
    Dimensionless
};

inline double convertParameterValue(const double value,
                                    const ParameterDimension dimension,
                                    const Constants::UnitSystem from,
                                    const Constants::UnitSystem to) {
    switch (dimension) {
        case ParameterDimension::Energy:
            return Constants::Convert::energy(value, from, to);
        case ParameterDimension::Length:
            return Constants::Convert::length(value, from, to);
        case ParameterDimension::Dimensionless:
            return value;
    }
    return value;
}

/// Описание одного вводимого параметра: по нему панель параметров сама строит поле ввода
struct ParameterSpec {
    QString key; // Идентификатор параметра в ParameterValues
    QString label; // Подпись в интерфейсе
    QString unit; // Единица измерения (суффикс поля ввода)
    ParameterDimension dimension; // Размерность для конвертации между системами единиц
    double minimum;
    double maximum;
    double defaultValue;
    double step;
    int significantDigits;
};

/// Значения параметров: ключ ParameterSpec::key -> значение
using ParameterValues = QMap<QString, double>;

/**
 * @brief Адаптер между UI и конкретной квантовой системой из ядра.
 *
 * Знает, какие параметры нужны системе и как из них создать QuantumSystem в выбранной UnitSystem.
 * Чтобы добавить новую систему (например, двойную яму), достаточно написать новый дескриптор
 * и зарегистрировать его в SystemRegistry — остальной UI менять не нужно.
 */
class SystemDescriptor {
public:
    virtual ~SystemDescriptor() = default;

    /// Название для списка выбора
    [[nodiscard]] virtual QString displayName() const = 0;

    /// Список параметров для построения формы ввода в заданной системе единиц
    [[nodiscard]] virtual std::vector<ParameterSpec> parameters(Constants::UnitSystem unitSystem) const = 0;

    /// Создаёт систему в заданной системе единиц. При недопустимых параметрах бросает std::invalid_argument
    [[nodiscard]] virtual std::unique_ptr<QuantumSystem> create(const ParameterValues &values,
                                                                Constants::UnitSystem unitSystem) const = 0;
};

#endif //RECTQUANTUMWELLS_SYSTEMDESCRIPTOR_HPP
