add_module_test(
    NAME plugin_tests
    SOURCES plugin_tests.cpp
    DEPENDS Catch2::Catch2WithMain plugin
)
