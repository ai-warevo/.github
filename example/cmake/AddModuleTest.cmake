# ── Add Module Test ───────────────────────────────────────────────────────
# Helper function to reduce boilerplate when registering module tests.
#
# Usage:
#   add_module_test(
#       NAME test_name
#       SOURCES test1.cpp test2.cpp
#       DEPENDS mylib
#       [LIBRARIES extra_lib1 extra_lib2]
#   )

function(add_module_test)
    cmake_parse_arguments(MOD_TEST "" "NAME" "SOURCES;DEPENDS;LIBRARIES" ${ARGN})

    if(NOT MOD_TEST_NAME)
        message(FATAL_ERROR "add_module_test: NAME is required")
    endif()

    if(NOT MOD_TEST_SOURCES)
        message(FATAL_ERROR "add_module_test: SOURCES is required")
    endif()

    if(NOT MOD_TEST_DEPENDS)
        message(FATAL_ERROR "add_module_test: DEPENDS is required (usually 'lib::module' or 'Catch2::Catch2')")
    endif()

    # Create executable
    add_executable(${MOD_TEST_NAME} ${MOD_TEST_SOURCES})

    # Link dependencies
    target_link_libraries(${MOD_TEST_NAME} PRIVATE ${MOD_TEST_DEPENDS})

    # Link optional extra libraries
    if(MOD_TEST_LIBRARIES)
        target_link_libraries(${MOD_TEST_NAME} PRIVATE ${MOD_TEST_LIBRARIES})
    endif()

    # Register with ctest
    add_test(NAME ${MOD_TEST_NAME} COMMAND ${MOD_TEST_NAME})

    # Set LD_LIBRARY_PATH for tests to find shared libraries
    if(UNIX)
        set_tests_properties(${MOD_TEST_NAME} PROPERTIES
            ENVIRONMENT "LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/lib/plugin:$ENV{LD_LIBRARY_PATH}"
        )
    endif()

endfunction()

