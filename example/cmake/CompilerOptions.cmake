# ── Compiler Options ──────────────────────────────────────────────────────
# Shared compilation flags and options across all modules.

# ── C++ Standard ───────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ── Visibility ─────────────────────────────────────────────────────────────
# Default to hidden visibility; symbols must be explicitly exported.
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

# ── Warnings ───────────────────────────────────────────────────────────────
# Strict warnings for production code; relaxed in tests (via local .clang-tidy).
if(MSVC)
    # MSVC: /W4 is strict; treat warnings as errors if requested
    add_compile_options(/W4)
    if(APP_WARNINGS_AS_ERRORS)
        add_compile_options(/WX)
    endif()
else()
    # GCC / Clang: -Wall -Wextra
    add_compile_options(-Wall -Wextra -Wpedantic)
    # Note: -Werror is NOT applied globally (affects FetchContent deps like Catch2)
    # Instead, it's applied per-target in CMakeLists.txt or via CMakePresets
endif()

# ── Sanitizer Options (inferred from preset name) ──────────────────────────
# This function applies sanitizer flags based on the preset name.
# Callers can also set AW_EXAMPLE_SANITIZE or pass them explicitly to CMake.
function(apply_sanitizers_from_preset target)
    string(TOLOWER "${CMAKE_BUILD_TYPE}${CMAKE_CXX_COMPILER_ID}" _config)
    
    if(CMAKE_BUILD_TYPE MATCHES ".*[Aa]san.*" OR CMAKE_CXX_FLAGS MATCHES ".*fsanitize=address.*")
        target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    elseif(CMAKE_BUILD_TYPE MATCHES ".*[Tt]san.*" OR CMAKE_CXX_FLAGS MATCHES ".*fsanitize=thread.*")
        target_compile_options(${target} PRIVATE -fsanitize=thread -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=thread)
    endif()
endfunction()

# ── Export Compiler Commands ───────────────────────────────────────────────
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
