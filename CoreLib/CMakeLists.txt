
add_library(CoreLib STATIC src/EqSolver.cpp
        include/IntegralCalculator.hpp)

add_library(CoreLib::CoreLib ALIAS CoreLib)

target_include_directories(CoreLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)