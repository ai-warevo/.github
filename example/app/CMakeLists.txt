# Host application
add_executable(host src/main.cpp)

target_link_libraries(host PRIVATE plugin_api platform)

target_compile_features(host PRIVATE cxx_std_23)

# Copy plugin library next to executable for convenience
add_custom_command(TARGET host POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin>
        $<TARGET_FILE_DIR:host>
    COMMENT "Copying plugin library next to host executable"
)
