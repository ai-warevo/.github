/**
 * @file main.cpp
 * @brief Host application for dynamic plugin loading.
 *
 * At runtime loads the plugin library (.so / .dll) and calls
 * exported public API functions.
 *
 * Platform-specific details are encapsulated in aw::example::platform.
 */

#include "platform/platform.h"
#include "plugin_api/plugin.h"

#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <tuple>

namespace aw::example::host {

using version_fn = int (*)();
using compute_fn = int (*)(int, int, int);
using version_str_fn = const char* (*)();
using op_name_fn = const char* (*)(int);

inline std::optional<
    std::pair<aw::example::platform::LibraryHandle, std::tuple<version_fn, compute_fn, version_str_fn, op_name_fn>>>
load_plugin() {
    auto lib_path = aw::example::platform::find_plugin();
    if (!lib_path.has_value()) {
        std::cerr << "[host] Cannot find plugin path\n";
        return {};
    }

    const auto& path = lib_path.value();
    if (path.empty()) {
        std::cout << "[host] Loading plugin from system path\n";
    } else {
        std::cout << std::format("[host] Loading plugin from: {}\n", path);
    }

    auto lib = aw::example::platform::load_library(path);
    if (!lib) {
        std::cerr << "[host] Failed to load plugin\n";
        return std::nullopt;
    }

    auto* raw_version = aw::example::platform::resolve_symbol(lib.value(), "plugin_version");
    auto* raw_compute = aw::example::platform::resolve_symbol(lib.value(), "plugin_compute");
    auto* raw_version_s = aw::example::platform::resolve_symbol(lib.value(), "plugin_version_string");
    auto* raw_op_name = aw::example::platform::resolve_symbol(lib.value(), "plugin_op_name");

    if (raw_version == nullptr || raw_compute == nullptr || raw_version_s == nullptr || raw_op_name == nullptr) {
        std::cerr << "[host] Failed to resolve plugin symbols\n";
        aw::example::platform::unload_library(lib.value());
        return std::nullopt;
    }

    auto fn_version = reinterpret_cast<version_fn>(raw_version);
    auto fn_compute = reinterpret_cast<compute_fn>(raw_compute);
    auto fn_version_s = reinterpret_cast<version_str_fn>(raw_version_s);
    auto fn_op_name = reinterpret_cast<op_name_fn>(raw_op_name);

    return std::make_pair(lib.value(), std::make_tuple(fn_version, fn_compute, fn_version_s, fn_op_name));
}

inline void run_plugin(aw::example::platform::LibraryHandle& lib,
                       const std::tuple<version_fn, compute_fn, version_str_fn, op_name_fn>& symbols) {
    const auto& [fn_version, fn_compute, fn_version_s, fn_op_name] = symbols;

    std::cout << "\n--- Plugin Info ---\n";
    std::cout << std::format("  version:     {}\n", fn_version());
    std::cout << std::format("  version_str: {}\n", fn_version_s());

    std::cout << "\n--- Operations ---\n";
    constexpr int test_a = 42;
    constexpr int test_b = 13;
    for (int op = 0; op <= 2; ++op) {
        std::cout << std::format("  {}({}, {})\n", fn_op_name(op), test_a, test_b)
                  << std::format("    = {}\n", fn_compute(test_a, test_b, op));
    }

    constexpr int unknown_op = 99;
    std::cout << std::format("  unknown({}, {}, {}) = {}\n", test_a, test_b, unknown_op, fn_compute(test_a, test_b, unknown_op));

    aw::example::platform::unload_library(lib);
}

} // namespace aw::example::host

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    std::cout << "=== Plugin Host ===" << '\n';

    auto result = aw::example::host::load_plugin();
    if (!result) {
        return 1;
    }

    aw::example::host::run_plugin(result->first, result->second);

    std::cout << "\n=== Done ===\n";
    return 0;
}
