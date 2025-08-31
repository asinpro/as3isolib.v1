# Find or download Catch2
find_package(Catch2 3 QUIET)

if(NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
    )
    FetchContent_MakeAvailable(Catch2)
endif()

# Test executable
add_executable(as3isolib_tests
    test_main.cpp
    test_pt.cpp
    test_events.cpp
    test_display.cpp
)

target_link_libraries(as3isolib_tests 
    PRIVATE 
        as3isolib_cpp 
        Catch2::Catch2WithMain
)

# Discover tests
include(CTest)
include(Catch)
catch_discover_tests(as3isolib_tests)