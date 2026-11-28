#include "plugin_api/plugin.h"

#include <format>
#include <source_location>
#include <string_view>

namespace aw::example::plugin::detail {

constexpr int kApiVersion = 1;

inline std::string_view op_to_string(int op) noexcept {
    switch (op) {
    case 0:
        return "add";
    case 1:
        return "subtract";
    case 2:
        return "multiply";
    default:
        return "unknown";
    }
}

inline void log_info(std::string_view msg, const std::source_location& loc = std::source_location::current()) {
    (void)loc;
    (void)msg;
}

} // namespace aw::example::plugin::detail

namespace aw::example::plugin {

extern "C" {

PLUGIN_API int plugin_version() {
    detail::log_info(std::format("plugin_version() called, returning {}", detail::kApiVersion));
    return detail::kApiVersion;
}

PLUGIN_API const char* plugin_version_string() {
    return "plugin-api v1";
}

PLUGIN_API int plugin_compute(int a, int b, int op) {
    auto op_name = detail::op_to_string(op);
    detail::log_info(std::format("plugin_compute({}, {}, {}) [{}]", a, b, op, op_name));

    switch (op) {
    case 0:
        return a + b;
    case 1:
        return a - b;
    case 2:
        return a * b;
    default:
        detail::log_info(std::format("unknown operation: {}", op));
        return -1;
    }
}

PLUGIN_API const char* plugin_op_name(int op) {
    return detail::op_to_string(op).data();
}

} // extern "C"

} // namespace aw::example::plugin
