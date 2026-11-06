add_executable(CoreLibTests
        src/testSolver.cpp
)
target_link_libraries(CoreLibTests PRIVATE CoreLib::CoreLib)
add_test(NAME CoreLibTests COMMAND CoreLibTests)