add_module_test(
    NAME plugin_tests
    SOURCES plugin_tests.cpp
    DEPENDS Catch2::Catch2WithMain plugin
)

# Copy plugin library next to test executable for runtime discovery
add_custom_command(TARGET plugin_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:plugin>
        $<TARGET_FILE_DIR:plugin_tests>
    COMMENT "Copying plugin library next to plugin_tests executable"
)

