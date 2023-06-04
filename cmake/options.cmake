#######
## PROJECT OPTIONS
#######


#
# Compiler options
#
option(${PROJECT_NAME}_WARNINGS_AS_ERRORS "Treat compiler warnings as errors." OFF)

#
# Unit testing
#
option(${PROJECT_NAME}_ENABLE_UNIT_TESTING "Enable unit tests for the projects (from the `test` subfolder)." OFF)
option(${PROJECT_NAME}_USE_CATCH2 "Use the Catch2 project for creating unit tests." OFF)


#
# Static analyzers
#
# Currently supporting: Clang-Tidy, Cppcheck.
option(${PROJECT_NAME}_ENABLE_CLANG_TIDY "Enable static analysis with Clang-Tidy." OFF)
option(${PROJECT_NAME}_ENABLE_CPPCHECK "Enable static analysis with Cppcheck." OFF)

#
# Doxygen
#
#option(${PROJECT_NAME}_ENABLE_DOXYGEN "Enable Doxygen documentation builds of source." OFF)

#
# Miscelanious options
#

# Generate compile_commands.json for clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(${PROJECT_NAME}_VERBOSE_OUTPUT "Enable verbose output, allowing for a better understanding of each step taken." ON)
option(${PROJECT_NAME}_GENERATE_EXPORT_HEADER "Create a `project_export.h` file containing all exported symbols." OFF)

# Export all symbols when building a shared library
if(BUILD_SHARED_LIBS)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
endif()

option(${PROJECT_NAME}_ENABLE_ASAN "Enable Address Sanitize to detect memory error." OFF)
if(${PROJECT_NAME}_ENABLE_ASAN)
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif()

