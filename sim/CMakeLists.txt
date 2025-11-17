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
)

target_link_libraries(sim_tests
    PRIVATE
        sim_pmsm
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sim_tests)
