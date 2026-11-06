//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_TOOLS_HPP
#define RECTQUANTUMWELLS_TOOLS_HPP

namespace Constants {
    enum class UnitSystem {
        Nano, // Энергия: эВ, Длина: нм (по умолчанию для задачи)
        SI, // Энергия: Дж, Длина: м
        CGS, // Энергия: эрг, Длина: см
        AtomicUnits // Энергия: Hartree, Длина: a_0 (Bohr radius)
    };

    // =========================================================================
    // 1. Система СИ (метры, килограммы, Джоули)
    // =========================================================================
    namespace SI {
        constexpr double HBAR = 1.054571817e-34; // Дж * с
        constexpr double ELECTRON_MASS = 9.1093837e-31; // кг
        constexpr double ELEMENTARY_CHARGE = 1.602176634e-19; // Кл (Дж/эВ)
        constexpr double HBAR2_DIV_2M = 6.10426e-39; // Дж * м^2
    }

    // =========================================================================
    // 2. Система СГС (сантиметры, граммы, эрги) — как в книге Абаренкова
    // =========================================================================
    namespace CGS {
        constexpr double HBAR = 1.054571817e-27; // эрг * с
        constexpr double ELECTRON_MASS = 9.1093837e-28; // г
        constexpr double ELEMENTARY_CHARGE = 4.8032047e-10; // ед. СГСЭ (statC)
        constexpr double EV_IN_ERG = 1.602176634e-12; // 1 эВ в эргах
        constexpr double HBAR2_DIV_2M = 6.10426e-28; // эрг * см^2
    }

    // =========================================================================
    // 3. Практическая система для наноструктур (нанометры, электронвольты)
    // =========================================================================
    namespace Nano {
        // hbar^2 / (2 * m_e) в [эВ * нм^2]
        constexpr double HBAR2_DIV_2M = 0.03809982;
    }

    // =========================================================================
    // 4. Атомная система единиц (Hartree, радиус Бора a_0)
    // =========================================================================
    namespace Atomic {
        constexpr double HARTREE_IN_EV = 27.211386245988; // 1 Ha в эВ
        constexpr double BOHR_IN_NM = 0.0529177210903; // 1 a_0 в нм
        constexpr double HBAR2_DIV_2M = 0.5; // в а.е. hbar = m_e = 1 -> 1/(2*1) = 0.5
    }

    // =========================================================================
    // 5. Формулы взаимного перевода величин
    // =========================================================================
    namespace Convert {
        // --- Энергия ---
        constexpr double eV_to_erg(double eV) { return eV * CGS::EV_IN_ERG; }
        constexpr double erg_to_eV(double erg) { return erg / CGS::EV_IN_ERG; }

        constexpr double J_to_erg(double joules) { return joules * 1e7; }
        constexpr double erg_to_J(double erg) { return erg * 1e-7; }
        constexpr double eV_to_J(double eV) { return eV * SI::ELEMENTARY_CHARGE; }
        constexpr double J_to_eV(double joules) { return joules / SI::ELEMENTARY_CHARGE; }

        constexpr double eV_to_Hartree(double eV) { return eV / Atomic::HARTREE_IN_EV; }
        constexpr double Hartree_to_eV(double hartree) { return hartree * Atomic::HARTREE_IN_EV; }

        // --- Длина ---
        constexpr double nm_to_cm(double nm) { return nm * 1e-7; }
        constexpr double cm_to_nm(double cm) { return cm * 1e7; }
        constexpr double m_to_cm(double m) { return m * 1e2; }
        constexpr double cm_to_m(double cm) { return cm * 1e-2; }
        constexpr double nm_to_m(double nm) { return nm * 1e-9; }
        constexpr double m_to_nm(double m) { return m * 1e9; }

        constexpr double nm_to_Bohr(double nm) { return nm / Atomic::BOHR_IN_NM; }
        constexpr double Bohr_to_nm(double bohr) { return bohr * Atomic::BOHR_IN_NM; }

        // --- Масса ---
        constexpr double kg_to_g(double kg) { return kg * 1e3; }
        constexpr double g_to_kg(double g) { return g * 1e-3; }

        // --- Коэффициент hbar^2 / (2m) из СГС в нано-единицы (эВ * нм^2) ---
        constexpr double hbar2_div_2m_cgs_to_nano(double val_cgs) {
            // (эрг * см^2) -> (эВ * нм^2)
            return (val_cgs * 1e14) / CGS::EV_IN_ERG;
        }

        // --- Универсальный перевод энергии между любыми двумя UnitSystem ---
        constexpr double energyToNano(double value, UnitSystem from) {
            switch (from) {
                case UnitSystem::Nano: return value;
                case UnitSystem::SI: return J_to_eV(value);
                case UnitSystem::CGS: return erg_to_eV(value);
                case UnitSystem::AtomicUnits: return Hartree_to_eV(value);
            }
            return value;
        }

        constexpr double energyFromNano(double value_eV, UnitSystem to) {
            switch (to) {
                case UnitSystem::Nano: return value_eV;
                case UnitSystem::SI: return eV_to_J(value_eV);
                case UnitSystem::CGS: return eV_to_erg(value_eV);
                case UnitSystem::AtomicUnits: return eV_to_Hartree(value_eV);
            }
            return value_eV;
        }

        constexpr double energy(double value, UnitSystem from, UnitSystem to) {
            if (from == to) return value;
            return energyFromNano(energyToNano(value, from), to);
        }

        // --- Универсальный перевод длины между любыми двумя UnitSystem ---
        constexpr double lengthToNano(double value, UnitSystem from) {
            switch (from) {
                case UnitSystem::Nano: return value;
                case UnitSystem::SI: return m_to_nm(value);
                case UnitSystem::CGS: return cm_to_nm(value);
                case UnitSystem::AtomicUnits: return Bohr_to_nm(value);
            }
            return value;
        }

        constexpr double lengthFromNano(double value_nm, UnitSystem to) {
            switch (to) {
                case UnitSystem::Nano: return value_nm;
                case UnitSystem::SI: return nm_to_m(value_nm);
                case UnitSystem::CGS: return nm_to_cm(value_nm);
                case UnitSystem::AtomicUnits: return nm_to_Bohr(value_nm);
            }
            return value_nm;
        }

        constexpr double length(double value, UnitSystem from, UnitSystem to) {
            if (from == to) return value;
            return lengthFromNano(lengthToNano(value, from), to);
        }
    }

    // Функция, возвращающая gamma = hbar^2 / (2m) для выбранной системы
    constexpr double getHbar2Div2M(UnitSystem system = UnitSystem::Nano, double effectiveMassRatio = 1.0) {
        double baseGamma = 0.0;
        switch (system) {
            case UnitSystem::Nano: baseGamma = Nano::HBAR2_DIV_2M;
                break; // эВ * нм^2
            case UnitSystem::SI: baseGamma = SI::HBAR2_DIV_2M;
                break; // Дж * м^2
            case UnitSystem::CGS: baseGamma = CGS::HBAR2_DIV_2M;
                break; // эрг * см^2
            case UnitSystem::AtomicUnits: baseGamma = Atomic::HBAR2_DIV_2M;
                break; // Ha * a_0^2
        }
        return baseGamma / effectiveMassRatio;
    }
} // namespace Constants

#endif //RECTQUANTUMWELLS_TOOLS_HPP
