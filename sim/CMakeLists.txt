enable_testing()

add_executable(sim_tests
    tests/test_pmsm.cpp
    src/pmsm.cpp
)

target_include_directories(sim_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/core/include
)

target_link_libraries(sim_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(sim_tests)