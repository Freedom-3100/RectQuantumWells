
add_library(CoreLib STATIC
        src/EqSolver.cpp
        src/SingleWellSolver.cpp
        src/SingleWellSystem.cpp
        include/EnergyLevel.hpp
        include/EqSolver.hpp
        include/IntegralCalculator.hpp
        include/QuantumSystem.hpp
        include/RootBracketFinder.hpp
        include/SingleWellSolver.hpp
        include/SingleWellSystem.hpp
        include/Tools.hpp)

add_library(CoreLib::CoreLib ALIAS CoreLib)

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
