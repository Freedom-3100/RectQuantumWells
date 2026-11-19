//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_ENERGYLEVEL_HPP
#define RECTQUANTUMWELLS_ENERGYLEVEL_HPP

struct EnergyLevel {
    int index; // Номер уровня: 0, 1, 2...
    double energy; // Энергия в единицах выбранной UnitSystem (-V0 < energy < 0)
    bool isEven; // true - чётное (симметричное), false - нечётное
};

#endif //RECTQUANTUMWELLS_ENERGYLEVEL_HPP

