# ── Example C++23 Plugin Project (Production-Ready Structure) ─────────────
cmake_minimum_required(VERSION 3.20)
project(example LANGUAGES CXX)

# ── Determine if this is a top-level build ──────────────────────────────────
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(AW_EXAMPLE_IS_TOP_LEVEL ON)
else()
    set(AW_EXAMPLE_IS_TOP_LEVEL OFF)
endif()

# ── Shared Compiler Options ──────────────────────────────────────────────────
include(cmake/CompilerOptions.cmake)
include(cmake/AddModuleTest.cmake)

# ── Library Modules (api, platform, plugin) ──────────────────────────────────
add_subdirectory(lib)

# ── Application (host executable) ────────────────────────────────────────────
add_subdirectory(app)

# ── Apply -Werror to our targets (not FetchContent dependencies) ──────────────
# This must happen BEFORE FetchContent is added, so our targets get it but Catch2 doesn't.
# ASan can trigger false-positive uninitialized warnings in libstdc++ regex code.
if(APP_WARNINGS_AS_ERRORS AND NOT MSVC)
    foreach(_tgt IN ITEMS plugin host)
        if(TARGET ${_tgt})
            target_compile_options(${_tgt} PRIVATE -Werror)
        endif()
    endforeach()
endif()

# ── Tests (opt-in, enabled by default for top-level builds) ──────────────────
option(AW_EXAMPLE_BUILD_TESTS "Build example unit tests" ${AW_EXAMPLE_IS_TOP_LEVEL})

if(AW_EXAMPLE_BUILD_TESTS)
    enable_testing()

    # ── Catch2 v3: local third_party first, FetchContent fallback ──────────
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/catch2/CMakeLists.txt")
        add_subdirectory(third_party/catch2)
    else()
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG        v3.16.1
            GIT_SHALLOW    TRUE
        )
        FetchContent_MakeAvailable(Catch2)
    endif()

    # ── Register module tests ────────────────────────────────────────────
    add_subdirectory(lib/plugin/tests)
    
    # ── Apply -Werror to test targets (after they're created) ──────────────
    if(APP_WARNINGS_AS_ERRORS AND NOT MSVC)
        if(TARGET plugin_tests)
            target_compile_options(plugin_tests PRIVATE -Werror)
        endif()
    endif()
endif()

# ── Coverage & Sanitizer Support (applied to project targets) ────────────────
# List all project targets (not FetchContent deps like Catch2)
set(_AW_PROJECT_TARGETS host plugin)

if(AW_EXAMPLE_BUILD_TESTS)
    list(APPEND _AW_PROJECT_TARGETS plugin_tests)
endif()

# Coverage instrumentation (mutually exclusive with sanitizers)
if(CMAKE_CXX_FLAGS MATCHES ".*--coverage.*" OR CMAKE_BUILD_TYPE MATCHES ".*coverage.*")
    foreach(_tgt IN LISTS _AW_PROJECT_TARGETS)
        if(TARGET ${_tgt})
            target_compile_options(${_tgt} PRIVATE --coverage -O0 -g -fprofile-update=atomic)
            target_link_options(${_tgt} PRIVATE --coverage)
        endif()
    endforeach()
endif()

# Sanitizer support (inferred from CMake flags or preset name)
if(CMAKE_CXX_FLAGS MATCHES ".*fsanitize=.*")
    # Flags are already set via CMakePresets.json; just apply_sanitizers_from_preset per target
    foreach(_tgt IN LISTS _AW_PROJECT_TARGETS)
        if(TARGET ${_tgt})
            apply_sanitizers_from_preset(${_tgt})
        endif()
    endforeach()
endif()
