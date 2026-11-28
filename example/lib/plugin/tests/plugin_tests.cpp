/**
 * @file plugin_tests.cpp
 * @brief Unit tests for the plugin via Catch2 v3.
 *
 * Tests all public API functions:
 * - plugin_version()
 * - plugin_version_string()
 * - plugin_compute(a, b, op)
 * - plugin_op_name(op)
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>
#include <string>
#include <string_view>

#include "plugin_api/plugin.h"

namespace aw::example::tests {

// ── plugin_version ────────────────────────────────────────────────────────

TEST_CASE("plugin_version returns 1", "[version]") {
    REQUIRE(aw::example::plugin::plugin_version() == 1);
}

// ── plugin_version_string ─────────────────────────────────────────────────

TEST_CASE("plugin_version_string returns non-empty", "[version]") {
    const auto* s = aw::example::plugin::plugin_version_string();
    REQUIRE(s != nullptr);
    REQUIRE(std::string(s).find("plugin-api") != std::string::npos);
}

// ── plugin_compute ────────────────────────────────────────────────────────

TEST_CASE("plugin_compute: add", "[compute]") {
    REQUIRE(aw::example::plugin::plugin_compute(2, 3, 0) == 5);
    REQUIRE(aw::example::plugin::plugin_compute(-1, 1, 0) == 0);
    REQUIRE(aw::example::plugin::plugin_compute(0, 0, 0) == 0);
}

TEST_CASE("plugin_compute: subtract", "[compute]") {
    REQUIRE(aw::example::plugin::plugin_compute(10, 3, 1) == 7);
    REQUIRE(aw::example::plugin::plugin_compute(0, 0, 1) == 0);
    REQUIRE(aw::example::plugin::plugin_compute(-5, -3, 1) == -2);
}

TEST_CASE("plugin_compute: multiply", "[compute]") {
    REQUIRE(aw::example::plugin::plugin_compute(3, 4, 2) == 12);
    REQUIRE(aw::example::plugin::plugin_compute(0, 99, 2) == 0);
    REQUIRE(aw::example::plugin::plugin_compute(-2, 3, 2) == -6);
}

TEST_CASE("plugin_compute: unknown op returns -1", "[compute]") {
    REQUIRE(aw::example::plugin::plugin_compute(1, 2, 99) == -1);
    REQUIRE(aw::example::plugin::plugin_compute(0, 0, -1) == -1);
}

// ── plugin_op_name ────────────────────────────────────────────────────────

TEST_CASE("plugin_op_name: valid ops", "[op_name]") {
    REQUIRE(std::string(aw::example::plugin::plugin_op_name(0)) == "add");
    REQUIRE(std::string(aw::example::plugin::plugin_op_name(1)) == "subtract");
    REQUIRE(std::string(aw::example::plugin::plugin_op_name(2)) == "multiply");
}

TEST_CASE("plugin_op_name: unknown op", "[op_name]") {
    REQUIRE(std::string(aw::example::plugin::plugin_op_name(99)) == "unknown");
}

} // namespace aw::example::tests
