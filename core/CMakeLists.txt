include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(core_tests
    tests/test_current_loop.cpp
    tests/test_foc_math.cpp
    tests/test_foc.cpp
    tests/test_modulation.cpp
    tests/test_pi.cpp
    tests/test_speed_loop.cpp
    src/current_loop.cpp
    src/foc.cpp
    src/modulation.cpp
    src/pi.cpp   
    src/speed_loop.cpp
)

target_include_directories(core_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(core_tests
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(core_tests)
