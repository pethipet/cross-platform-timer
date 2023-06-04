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

#
# Project options
# 
include ( cmake/options.cmake )

#
# Generic functions for C projects
# 
include ( cmake/my_c.cmake )


#
# Some utils 
# 
include ( cmake/utils.cmake )




#this will allow using the  same _DEBUG macro available in both Linux as well as Windows - MSCV
if (UNIX)
    add_compile_options("$<$<CONFIG:DEBUG>:-D_DEBUG>")    
endif (UNIX)


# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

message(STATUS "CMake for ${PROJECT_NAME} v${PROJECT_VERSION}...\n")

#
# Prevent building in the source directory
#
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds not allowed. Please make a new directory and run CMake from there.\n")
endif()


#
# Create library, setup header and source files
#

#
# Specify  all headers and implementation files
#
set(headers
    crosstimer.h
)

set(sources
    crosstimer.c
)

set(exesources
		main.c
		${sources}
)

set(test_sources
  crosstimer_test.c
)

#
if(${PROJECT_NAME}_BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME} ${exesources})

  if(${PROJECT_NAME}_VERBOSE_OUTPUT)
    v_message("Found the following sources:")
    foreach(source IN LISTS exesources)
      v_message("* ${source}")
    endforeach()
  endif()

  if(${PROJECT_NAME}_ENABLE_UNIT_TESTING)
    add_library(${PROJECT_NAME}_LIB ${headers} ${sources})

    if(${PROJECT_NAME}_VERBOSE_OUTPUT)
      v_message("Found the following headers:")
      foreach(header IN LISTS headers)
        v_message("* ${header}")
      endforeach()
    endif()
  endif()
else()
  add_library(
    ${PROJECT_NAME}
    ${headers}
    ${sources}
  )

  if(${PROJECT_NAME}_VERBOSE_OUTPUT)
  v_message("Found the following sources:")
    foreach(source IN LISTS sources)
      v_message("* ${source}")
    endforeach()
    v_message("Found the following headers:")
    foreach(header IN LISTS headers)
      v_message("* ${header}")
    endforeach()
  endif()
endif()


set_target_properties(
  ${PROJECT_NAME}
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)
if(${PROJECT_NAME}_BUILD_EXECUTABLE AND ${PROJECT_NAME}_ENABLE_UNIT_TESTING)
  set_target_properties(
    ${PROJECT_NAME}_LIB
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
    OUTPUT_NAME ${PROJECT_NAME}
  )
endif()

message(STATUS "Added all header and implementation files.\n")




if(UNIX AND NOT APPLE)
    # for Linux, BSD, maybe others
target_link_libraries(cross_timer rt)
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

