# Plugin API: Header-only public interface
add_library(plugin_api INTERFACE)

target_include_directories(plugin_api
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(plugin_api INTERFACE cxx_std_23)
