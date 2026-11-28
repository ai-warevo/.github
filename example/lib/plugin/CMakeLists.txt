# Plugin library
add_library(plugin SHARED src/plugin.cpp)

target_compile_definitions(plugin PRIVATE PLUGIN_BUILDING_DLL)

target_link_libraries(plugin PUBLIC plugin_api)

target_include_directories(plugin
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../plugin_api/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(plugin PUBLIC cxx_std_23)

set_target_properties(plugin PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

if(WIN32)
    set_target_properties(plugin PROPERTIES
        PREFIX ""
        SUFFIX ".dll"
    )
endif()
