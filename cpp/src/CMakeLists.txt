# Create the main library
set(SOURCES
    # Flash events
    flash/events/Event.cpp
    
    # Flash display
    flash/display/DisplayObject.cpp
    
    # as3isolib events
    as3isolib/events/IsoEvent.cpp
)

# Add the library
add_library(as3isolib_cpp STATIC ${SOURCES})

# Set target properties
target_include_directories(as3isolib_cpp 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link with raylib
if(RAYLIB_FOUND)
    target_link_libraries(as3isolib_cpp ${RAYLIB_LIBRARIES})
    target_include_directories(as3isolib_cpp PRIVATE ${RAYLIB_INCLUDE_DIRS})
    target_compile_options(as3isolib_cpp PRIVATE ${RAYLIB_CFLAGS_OTHER})
elseif(raylib_FOUND OR TARGET raylib)
    target_link_libraries(as3isolib_cpp raylib)
endif()

# Set C++ standard
target_compile_features(as3isolib_cpp PUBLIC cxx_std_17)