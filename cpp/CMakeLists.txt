cmake_minimum_required(VERSION 3.15)

project(as3isolib_cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(PkgConfig REQUIRED)

# Try to find raylib via pkg-config first, then try find_package
pkg_check_modules(RAYLIB raylib)
if(NOT RAYLIB_FOUND)
    find_package(raylib QUIET)
endif()

# If raylib is not found, download and build it
if(NOT RAYLIB_FOUND AND NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_MakeAvailable(raylib)
endif()

# Add subdirectories
add_subdirectory(src)

# Option to build tests
option(BUILD_TESTS "Build unit tests" ON)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Option to build examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()