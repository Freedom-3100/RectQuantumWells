# На macOS Homebrew ставит Qt6 в /opt/homebrew (Apple Silicon) или /usr/local (Intel)
if (APPLE)
    list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew" "/usr/local")
endif ()

find_package(Qt6 REQUIRED COMPONENTS Widgets PrintSupport)

# --- QCustomPlot: библиотека графиков (qcustomplot.h + qcustomplot.cpp, без собственного CMakeLists) ---
include(FetchContent)
FetchContent_Declare(
        qcustomplot
        URL https://www.qcustomplot.com/release/2.1.1/QCustomPlot-source.tar.gz
        URL_HASH SHA256=5e2d22dec779db8f01f357cbdb25e54fbcf971adaee75eae8d7ad2444487182f
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(qcustomplot)

add_library(QCustomPlot STATIC
        ${qcustomplot_SOURCE_DIR}/qcustomplot.cpp
        ${qcustomplot_SOURCE_DIR}/qcustomplot.h
)

target_include_directories(QCustomPlot SYSTEM PUBLIC ${qcustomplot_SOURCE_DIR})
target_compile_options(QCustomPlot PRIVATE -Wno-deprecated-declarations)
target_link_libraries(QCustomPlot PUBLIC Qt6::Widgets Qt6::PrintSupport)
set_target_properties(QCustomPlot PROPERTIES AUTOMOC ON)


add_executable(QuantumWellsUI
        src/main.cpp
        src/MainWindow.cpp
        src/MainWindow.hpp
        src/ParametersPanel.cpp
        src/ParametersPanel.hpp
        src/WellPlot.cpp
        src/WellPlot.hpp
        src/LevelsTable.cpp
        src/LevelsTable.hpp
        src/SystemDescriptor.hpp
        src/SingleWellDescriptor.cpp
        src/SingleWellDescriptor.hpp
        src/SystemRegistry.cpp
        src/SystemRegistry.hpp
        src/PlotSampling.hpp
        src/UnitLabels.hpp
        src/UiConfig.hpp
)
set_target_properties(QuantumWellsUI PROPERTIES AUTOMOC ON)

target_link_libraries(QuantumWellsUI
        PRIVATE
        CoreLib::CoreLib
        QCustomPlot
        Qt6::Widgets
)
