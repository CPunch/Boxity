cmake_minimum_required(VERSION 3.13)
project(Boxity)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/vendor/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

# Output binaries to the bin folder in the source directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Put CMake targets (ALL_BUILD/ZERO_CHECK) into a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the project as the default startup project for VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT boxity)

# find sfml
#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# adds address sanitizer support :)
find_package(Sanitizers)

include_directories(src ${SFML_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/vendor/Box2D/include/box2d)
file(GLOB_RECURSE SOURCES src/**.[ch]pp)

# add lua
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/Lua)

# add box2d
option(BOX2D_BUILD_UNIT_TESTS "" OFF)
option(BOX2D_BUILD_TESTBED "" OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/Box2D)

# add pugixml
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/pugixml)

add_executable(boxity ${SOURCES})
target_link_libraries(boxity lua_static sfml-graphics sfml-audio box2d pugixml)
add_sanitizers(boxity)
set_target_properties(boxity PROPERTIES OUTPUT_NAME boxity)

# Makes it so config, tdata, etc. get picked up when starting via the debugger in VS
set_property(TARGET boxity PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")