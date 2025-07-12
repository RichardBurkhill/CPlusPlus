cmake_minimum_required(VERSION 3.15)
project(C++14demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(c++14 C++14.cpp)

cmake_minimum_required(VERSION 3.14)
project(Cpp14DemoTests)

# Enable testing framework
enable_testing()

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set GTest paths (change if you’re not using /opt/homebrew)
set(GTEST_ROOT /opt/homebrew/opt/googletest)

# Add include directories for GTest/GMock
include_directories(${GTEST_ROOT}/include)
link_directories(${GTEST_ROOT}/lib)

# Add your test source file
add_executable(Cpp14DemoTests GMockExample.cpp)

# Link against GTest, GMock, and pthread
target_link_libraries(Cpp14DemoTests
    gtest
    gmock
    gtest_main
    pthread
)

# Enable CTest if desired
add_test(NAME Cpp14DemoTests COMMAND Cpp14DemoTests)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
add_executable(GeoTransformTests test_GeoTransform.cpp GeoTransform.cpp)
target_link_libraries(GeoTransformTests GTest::GTest GTest::Main pthread)
add_test(NAME GeoTransformTests COMMAND GeoTransformTests)