//
// Created by Mikhail on 06.11.2026.
//

#ifndef RECTQUANTUMWELLS_SYSTEMREGISTRY_HPP
#define RECTQUANTUMWELLS_SYSTEMREGISTRY_HPP

#include <memory>
#include <vector>

#include "SystemDescriptor.hpp"

/// Список всех систем, доступных в интерфейсе. Новая система добавляется одной строкой в .cpp
std::vector<std::shared_ptr<const SystemDescriptor>> availableSystems();

#endif //RECTQUANTUMWELLS_SYSTEMREGISTRY_HPP

