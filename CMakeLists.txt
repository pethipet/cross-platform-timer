##
##  Copyright 2023 Peter Himmler
##
##  Licensed under the Apache License, Version 2.0 (the "License");
##  you may not use this file except in compliance with the License.
##  You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
##  Unless required by applicable law or agreed to in writing, software
##  distributed under the License is distributed on an "AS IS" BASIS,
##  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
##  See the License for the specific language governing permissions and
##  limitations under the License.

# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version

cmake_minimum_required(VERSION 3.5)

#
# Project details
#
project(
  "cross_timer"
  VERSION 0.2.0
  LANGUAGES C
)

#######
## PROJECT OPTIONS
#######

option(${PROJECT_NAME}_BUILD_EXECUTABLE "Build the project as an executable, rather than a library." ON )
option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors." OFF)
option(${PROJECT_NAME}_VERBOSE_OUTPUT "Enable verbose output, allowing for a better understanding of each step taken." ON)
option(${PROJECT_NAME}_GENERATE_EXPORT_HEADER "Create a `project_export.h` file containing all exported symbols." OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install/${PROJECT_NAME}" CACHE PATH "Install path prefix" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)


# Generate compile_commands.json for clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Export all symbols when building a shared library
if(BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
  set(CMAKE_C_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()


# this allows using the  same _DEBUG macro available in both Linux as well as Windows - MSCV
if (UNIX)
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")    
endif (UNIX)

#
# Generic functions for C projects
#    
include ( cmake/my_c.cmake )

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

#
# Prevent building in the source directory
#
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there.\n")
endif()


#
# Specify  all headers and implementation files
#
set(headers
    src/crosstimer.h
)

set(sources
    src/crosstimer.c
)

set( exesources 
    main.c 
    ${sources}
)

if(${PROJECT_NAME}_VERBOSE_OUTPUT)
  v_message("Found the following headers:")
  foreach(source IN LISTS headers)
    v_message("* ${source}")
  endforeach()

  v_message("Found the following sources:")
  foreach(source IN LISTS sources)
    v_message("* ${source}")
  endforeach()

  v_message("Found the following exesources:")
  foreach(source IN LISTS exesources)
    v_message("* ${source}")
  endforeach()
endif()


#
add_executable(${PROJECT_NAME} 
  ${exesources}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)



# Library
add_library( ${PROJECT_NAME}_LIB
  ${headers}
  ${sources}
)

target_include_directories(${PROJECT_NAME}_LIB
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

set_target_properties(
  ${PROJECT_NAME}_LIB
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  OUTPUT_NAME ${PROJECT_NAME}
)

message(STATUS "Added all header and implementation files.\n")




if(UNIX AND NOT APPLE)
    # for Linux, BSD, maybe others
target_link_libraries(${PROJECT_NAME} rt)
endif()




#
# Compiler depend default warnings
#
set_project_warnings(${PROJECT_NAME})

#
# Set the project standard and warnings
#
set(CMAKE_C_STANDARD 11)


set_project_warnings(${PROJECT_NAME})

v_message("Applied compiler warnings. Using standard ${CMAKE_CXX_STANDARD}.\n")

