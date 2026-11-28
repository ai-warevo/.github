# Platform abstraction library
set(platform_sources)

if(WIN32)
    list(APPEND platform_sources src/platform_windows.cpp)
else()
    list(APPEND platform_sources src/platform_unix.cpp)
endif()

add_library(platform ${platform_sources})

target_include_directories(platform
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(platform PUBLIC cxx_std_23)

# Linux: dynamic loading requires -ldl
if(UNIX AND NOT APPLE)
    find_library(DL_LIBRARY dl)
    if(DL_LIBRARY)
        target_link_libraries(platform PUBLIC ${DL_LIBRARY})
    endif()
endif()
