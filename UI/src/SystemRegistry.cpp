//
// Created by Mikhail on 06.11.2026.
//

#include "SystemRegistry.hpp"

#include "SingleWellDescriptor.hpp"

std::vector<std::shared_ptr<const SystemDescriptor>> availableSystems() {
    return {
        std::make_shared<const SingleWellDescriptor>(),
        // Сюда добавится std::make_shared<const DoubleWellDescriptor>()
    };
}

