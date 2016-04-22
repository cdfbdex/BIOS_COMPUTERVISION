cmake_minimum_required(VERSION 2.6)
project(BIOSMABE)

# Set variables
include(${CMAKE_SOURCE_DIR}/cmake/SetEnv.cmake)

# Library sources
add_subdirectory(${LIBRARY_FOLDER})

# Library examples
add_subdirectory(examples)

# GUI: Graphics User Interface
add_subdirectory(gui)

# Install targets
include(${CMAKE_SOURCE_DIR}/cmake/InstallConfig.cmake)
