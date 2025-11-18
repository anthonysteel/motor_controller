add_library(sim_pmsm STATIC
    src/pmsm.cpp
)

target_include_directories(sim_pmsm
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/core/include
)

enable_testing()

add_executable(sim_tests
    tests/test_pmsm.cpp
    tests/test_closed_loop_position.cpp
    src/pmsm.cpp
    src/sim_axis_runner.cpp
)

target_include_directories(sim_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/core/include
)

target_link_libraries(sim_tests
    PRIVATE
        sim_pmsm
        core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sim_tests)
