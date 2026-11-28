#ifndef AW_EXAMPLE_PLUGIN_API_PLUGIN_H
#define AW_EXAMPLE_PLUGIN_API_PLUGIN_H

#include <string_view>

// ── Cross-platform export decorator ────────────────────────────────
#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef PLUGIN_BUILDING_DLL
#    define PLUGIN_API __declspec(dllexport)
#  else
#    define PLUGIN_API __declspec(dllimport)
#  endif
#else
#  define PLUGIN_API __attribute__((visibility("default")))
#endif

/**
 * @namespace aw::example::plugin
 * @brief Plugin public API.
 *
 * All exported functions use extern "C" for compatibility with
 * dynamic loaders (dlopen / LoadLibrary).
 */
namespace aw::example::plugin {

/**
 * @brief Returns the current API version.
 * @return API version (current: 1).
 */
extern "C" PLUGIN_API int plugin_version();

/**
 * @brief Returns a string representation of the API version.
 * @return Pointer to string of format "plugin-api v<N>".
 */
extern "C" PLUGIN_API const char* plugin_version_string();

/**
 * @brief Computes op(a, b).
 *
 * @param a First operand.
 * @param b Second operand.
 * @param op Operation code: 0 = add, 1 = subtract, 2 = multiply.
 * @return Result, or -1 for an unknown operation.
 */
extern "C" PLUGIN_API int plugin_compute(int a, int b, int op);

/**
 * @brief Returns the string name of an operation code.
 *
 * @param op Operation code.
 * @return "add", "subtract", "multiply", or "unknown".
 */
extern "C" PLUGIN_API const char* plugin_op_name(int op);

} // namespace aw::example::plugin

#endif  // AW_EXAMPLE_PLUGIN_API_PLUGIN_H
