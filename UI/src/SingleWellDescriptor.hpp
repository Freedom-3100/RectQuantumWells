//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_SINGLEWELLDESCRIPTOR_HPP
#define RECTQUANTUMWELLS_SINGLEWELLDESCRIPTOR_HPP

#include "SystemDescriptor.hpp"

/// Дескриптор одиночной прямоугольной ямы (параметры: глубина V0 и полуширина a)
class SingleWellDescriptor final : public SystemDescriptor {
public:
    [[nodiscard]] QString displayName() const override;

    [[nodiscard]] std::vector<ParameterSpec> parameters(Constants::UnitSystem unitSystem) const override;

    [[nodiscard]] std::unique_ptr<QuantumSystem> create(const ParameterValues &values,
                                                        Constants::UnitSystem unitSystem) const override;
};

#endif //RECTQUANTUMWELLS_SINGLEWELLDESCRIPTOR_HPP
