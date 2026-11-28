#ifndef AW_EXAMPLE_PLATFORM_PLATFORM_H
#define AW_EXAMPLE_PLATFORM_PLATFORM_H

#include <optional>
#include <string>
#include <string_view>

/**
 * @namespace aw::example::platform
 * @brief Cross-platform abstraction for dynamic library loading.
 *
 * Encapsulates platform-specific details:
 * - Library loading (LoadLibrary / dlopen)
 * - Symbol resolution (GetProcAddress / dlsym)
 * - Library path lookup
 */
namespace aw::example::platform {

/**
 * @brief Opaque handle for a loaded dynamic library.
 */
struct LibraryHandle {
    void* handle = nullptr;
};

/**
 * @brief Loads a library by path.
 *
 * @param path Library path; empty string = system search.
 * @return Handle on success, std::nullopt on error.
 */
std::optional<LibraryHandle> load_library(const std::string& path);

/**
 * @brief Unloads a library handle.
 *
 * @param lib Library handle (modified to null after unload).
 */
void unload_library(LibraryHandle& lib);

/**
 * @brief Resolves a symbol from a loaded library.
 *
 * @param lib Library handle.
 * @param name Symbol name.
 * @return Pointer to symbol on success, nullptr on error.
 */
void* resolve_symbol(LibraryHandle& lib, std::string_view name);

/**
 * @brief Searches for the plugin library.
 *
 * Order: ENV PLUGIN_PATH → next to executable → system path.
 *
 * @return Library path or std::nullopt if not found.
 *         Empty string = system search.
 */
std::optional<std::string> find_plugin();

} // namespace aw::example::platform

#endif  // AW_EXAMPLE_PLATFORM_PLATFORM_H
