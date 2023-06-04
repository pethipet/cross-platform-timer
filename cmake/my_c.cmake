

#
# Print a message only if the `VERBOSE_OUTPUT` option is on
#

function(v_message content)
    if(${PROJECT_NAME}_VERBOSE_OUTPUT)
			message(STATUS ${content})
    endif()
endfunction()

#
#
#
function(set_project_warnings project_name)
  set(MSVC_WARNINGS
      /W4     # Baseline reasonable warnings
      /permissive- # standards conformance mode for MSVC compiler.
      /w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
  )

  set(CLANG_WARNINGS
      -Wall
      -Wextra  # reasonable and standard
      -Wshadow # warn the user if a variable declaration shadows one from a
               # parent context
      -Wunused         # warn on anything being unused
      -Wconversion # warn on type conversions that may lose data
      -Wsign-conversion  # warn on sign conversions
      -Wformat=2 # warn on security issues around functions that format output (eg. printf)
  )

  set(GCC_WARNINGS
      ${CLANG_WARNINGS}
  )

  if (${PROJECT_NAME}_WARNINGS_AS_ERRORS)
    set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
    set(GCC_WARNINGS ${MSVC_WARNINGS} /WX)
  endif()

  if(MSVC)
    set(PROJECT_WARNINGS ${MSVC_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    set(PROJECT_WARNINGS ${CLANG_WARNINGS})
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(PROJECT_WARNINGS ${GCC_WARNINGS})
  else()
    message(AUTHOR_WARNING "No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}' compiler.")
  endif()

  target_compile_options(${project_name} PUBLIC ${PROJECT_WARNINGS})

  if(NOT TARGET ${project_name})
    message(AUTHOR_WARNING "${project_name} is not a target, thus no compiler warning were added.")
  endif()
endfunction()

