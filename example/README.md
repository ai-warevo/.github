# Example: Production-Ready C++23 Plugin System

A reference implementation of a modular, production-ready C++23 project structure suitable for reuse in consumer repositories.

## Directory Structure

```
example/
├── cmake/
│   ├── CompilerOptions.cmake        # Shared compilation flags: -Wall, -Wextra, sanitizers
│   └── AddModuleTest.cmake          # Test registration helper
├── lib/
│   ├── plugin_api/                  # Header-only public API contract
│   │   ├── include/plugin_api/plugin.h
│   │   └── CMakeLists.txt
│   ├── platform/                    # OS abstraction (Unix/Windows)
│   │   ├── include/platform/platform.h
│   │   ├── src/platform_{unix,windows}.cpp
│   │   ├── tests/                   # (optional: add platform-specific tests here)
│   │   └── CMakeLists.txt
│   ├── plugin/                      # Plugin implementation
│   │   ├── src/plugin.cpp
│   │   ├── tests/plugin_tests.cpp
│   │   ├── tests/.clang-tidy        # Relaxed linting for tests
│   │   └── CMakeLists.txt
│   ├── CMakeLists.txt
│   └── lib/CMakeLists.txt           # Aggregates all modules
├── app/
│   ├── src/main.cpp                 # Host application entry point
│   └── CMakeLists.txt
├── CMakeLists.txt                   # Root: includes cmake/, subdirs, testing
├── CMakePresets.json                # Build, test, and coverage presets
├── Makefile                         # Reusable CI/CD targets
├── .clang-format, .clang-tidy       # Code quality rules
├── Dockerfile, .dockerignore, .gitignore
└── README.md                        # This file
```

## Architecture

### Module Organization

1. **plugin_api** (header-only)
   - Public API contract: exported `extern "C"` functions
   - Zero dependencies
   - Versioned: pinned to v1

2. **platform** (library)
   - Cross-platform abstraction for dynamic library loading
   - Encapsulates POSIX (`dlopen`, `dlsym`) and Windows (`LoadLibrary`, `GetProcAddress`)
   - Conditional compilation: Unix vs. Windows implementations only

3. **plugin** (shared library)
   - Implements the plugin_api contract
   - Depends on plugin_api (public) only
   - Exports `PLUGIN_API` symbols via `extern "C"`
   - Tests use Catch2 v3 with relaxed linting rules

4. **app** (executable)
   - Host application that loads and invokes the plugin
   - Depends on plugin_api and platform
   - Runtime: copies plugin library to output directory for convenience

### Linting Strategy

- **Root `.clang-tidy`**: Strict production rules (all checks enabled)
- **`lib/*/tests/.clang-tidy`**: Inherits root, disables test-specific noise:
  - Function cognitive complexity (test helpers are allowed to be complex)
  - Magic numbers (test fixtures use literals)
  - Catch2 assertion macros (use global objects, standard for test frameworks)

## Building & Testing

### Quick Start

```bash
# Configure + build
cmake --preset gcc-15-debug
cmake --build build/gcc-15-debug --parallel

# Run tests
ctest --preset gcc-15-debug --output-on-failure

# Using make (recommended for CI)
make build PRESET=gcc-15-debug
make test PRESET=gcc-15-debug
```

### Supported Presets

| Preset | Compiler | Flags | Use Case |
|--------|----------|-------|----------|
| `gcc-15-debug` | g++-15 | `-O0 -g` | Development, debugging |
| `gcc-15-asan` | g++-15 | `-fsanitize=address,undefined` | Memory safety checks |
| `gcc-15-tsan` | g++-15 | `-fsanitize=thread` | Race condition detection |
| `gcc-15-coverage` | g++-15 | `--coverage` | Code coverage analysis |
| `clang18-libcxx-debug` | clang++-18 | `-stdlib=libc++` | Clang + libc++ |
| `clang18-libstdcxx-debug` | clang++-18 | `-stdlib=libstdc++` | Clang + libstdc++ |
| `msvc-debug` | MSVC | `/W4` | Windows builds |

### Coverage

```bash
make coverage PRESET=gcc-15-coverage

# Output: htmlcov/index.html (browser-friendly report)
#         coverage.xml (machine-readable)
```

Customize coverage filters by overriding the Makefile variable:

```bash
make coverage PRESET=gcc-15-coverage \
  GCOVR_FILTERS="lib/plugin/src lib/platform/src" \
  GCOVR_EXCLUDES="_deps third_party"
```

### Sanitizers

**AddressSanitizer + UBSan:**
```bash
make build PRESET=gcc-15-asan
make test PRESET=gcc-15-asan
```

**ThreadSanitizer:**
```bash
make build PRESET=gcc-15-tsan
make test PRESET=gcc-15-tsan  # Uses setarch on Linux to disable ASLR
```

### Linting

```bash
# Lint all source files (strict)
make lint

# Lint staged files only (pre-commit hook)
make lint-staged
```

## Adapting to Your Project

### Step 1: Copy & Rename

```bash
cp -r example/ myproject/
cd myproject/
```

### Step 2: Update Paths in Makefile

Edit `Makefile` to match your directory structure:

```makefile
# Change these variables to match your module layout:
GCOVR_FILTERS ?= mylib/src mylib/tests myapp/src      # your paths
GCOVR_EXCLUDES ?= third_party vendor _deps            # your exclusions
```

### Step 3: Update CMake Hierarchies

Update `lib/CMakeLists.txt` to include your modules:

```cmake
# Instead of:
add_subdirectory(plugin_api)
add_subdirectory(platform)
add_subdirectory(plugin)

# Use:
add_subdirectory(mylib_api)
add_subdirectory(mylib_core)
add_subdirectory(myapp)
```

### Step 4: Update Root CMakeLists.txt

If your structure differs (e.g., tests in `tests/` instead of per-module), adjust:

```cmake
add_subdirectory(lib)      # Your library modules
add_subdirectory(app)      # Your applications
add_subdirectory(tests)    # Optional: central test directory
```

### Step 5: Adjust .clang-tidy

The root `.clang-tidy` is comprehensive. For test directories, copy or link `lib/plugin/tests/.clang-tidy` to your test directories.

## Key Design Decisions

### Header-Only API (`plugin_api/`)

- Decouples the contract from implementation
- Allows multiple implementations of the same API
- Zero build-time cost for consumers
- Version stable: breaking changes create new directories (e.g., `plugin_api_v2/`)

### Platform Abstraction

- Unix (POSIX) and Windows implementations coexist in the same module
- Conditional compilation at CMake level (not via `#ifdef`)
- Cleaner separation: implementation details stay in `src/`, headers in `include/`

### Module-Scoped Testing

- Tests live beside the module they test
- Each module can disable production linting rules locally (via `.clang-tidy`)
- Catch2 v3 for modern C++ testing without macros

### Reusable Makefile

All build parameters are variables — override at the top level, never hardcoded in rules. This allows the Makefile to be copied into unrelated projects with only variable edits.

## CI/CD Integration

The Makefile and CMake structure are designed for seamless integration with GitHub Actions or other CI systems:

```yaml
- name: Build & Test
  run: |
    make build PRESET=gcc-15-debug
    make test PRESET=gcc-15-debug
    make lint

- name: Coverage
  run: make coverage PRESET=gcc-15-coverage
```

## Troubleshooting

**"Manually-specified variables were not used"** (CMAKE warning)
- The preset may define variables (`APP_ENABLE_WARNINGS`) that your CMakeLists.txt doesn't consume.
- Safe to ignore, or remove unused cache variables from `CMakePresets.json`.

**Coverage report is empty** (WARNING: All coverage data is filtered out)
- Verify `GCOVR_FILTERS` match your actual directory structure.
- Ensure tests actually exercise the code being measured.

**Plugin fails to load at runtime**
- Verify `PLUGIN_PATH` env var, or place library next to executable.
- Check platform-specific library suffixes: `.so` (Linux), `.dylib` (macOS), `.dll` (Windows).

## References

- [CMake: Functions](https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html)
- [Catch2 v3](https://github.com/catchorg/Catch2/tree/devel)
- [clang-tidy Checks](https://clang.llvm.org/extra/clang-tidy/checks/list.html)
- [gcovr](https://gcovr.com/)
