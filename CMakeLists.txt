cmake_minimum_required(VERSION 3.21)
project(SetContainer)

set(CMAKE_CXX_STANDARD 20)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



enable_testing()

add_executable(SetContainer rbtree.hpp set_container.hpp set_container.h tests.cpp)
target_link_libraries(
        SetContainer
        GTest::gtest_main
)