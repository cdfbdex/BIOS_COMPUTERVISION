# Set PROJECT_NAME_UPPERCASE and PROJECT_NAME_LOWERCASE variables
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPERCASE)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWERCASE)

# Version variables
set(MAJOR_VERSION 0)
set(MINOR_VERSION 1)
set(PATCH_VERSION 0)
set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR share/${PROJECT_NAME})
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Set up include-directories
include_directories("${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")

# Library name (by default is the project name in lowercase)
# Example: libbiosmabe.so
if(NOT BIOSMABE_LIBS)
  set(BIOSMABE_LIBS ${PROJECT_NAME_LOWERCASE})
endif()

# Library folder name (by default is the project name in lowercase)
# Example: #include <biosmabe/biosmabe.h>
if(NOT LIBRARY_FOLDER)
  set(LIBRARY_FOLDER ${PROJECT_NAME_LOWERCASE})
endif()

# The export set for all the targets
set(PROJECT_EXPORT ${PROJECT_NAME}EXPORT)

# Path of the CMake files generated
#set(PROJECT_CMAKE_FILES ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY})
set(PROJECT_CMAKE_FILES ${PROJECT_BINARY_DIR})

# The RPATH to be used when installing
set(CMAKE_INSTALL_RPATH ${INSTALL_LIB_DIR})
