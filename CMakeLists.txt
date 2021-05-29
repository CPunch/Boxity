cmake_minimum_required(VERSION 3.13)
project(SimplEngi)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake-modules/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})

# Output binaries to the bin folder in the source directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_VERBOSE_MAKEFILE on)

# Put CMake targets (ALL_BUILD/ZERO_CHECK) into a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the project as the default startup project fodr VS
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT simplengi)

# find sfml
#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

# adds address sanitizer support :)
find_package(Sanitizers)

include_directories(src ${SFML_INCLUDE_DIR})
file(GLOB_RECURSE SOURCES src/**.[ch]pp)

# add lua
add_subdirectory(${PROJECT_SOURCE_DIR}/cmake-modules/Lua)

add_executable(simplengi ${SOURCES})
target_link_libraries(simplengi lua_static sfml-graphics sfml-audio box2d)
add_sanitizers(simplengi)
set_target_properties(simplengi PROPERTIES OUTPUT_NAME simplengi)

# Makes it so config, tdata, etc. get picked up when starting via the debugger in VS
set_property(TARGET simplengi PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")