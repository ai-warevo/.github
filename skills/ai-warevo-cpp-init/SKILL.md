---
name: ai-warevo-cpp-init
description: Create a new C++23 project from template_cpp generator
inputs:
  project_name:
    type: string
    required: true
    description: "Project name (lowercase, e.g. my-lib)"
    pattern: "^[a-z][a-z0-9_-]*$"
  project_type:
    type: string
    required: true
    description: "Template type"
    enum: [cli, static, dynamic]
  namespace:
    type: string
    required: false
    description: "C++ namespace (default: aw::unnamed)"
    default: "aw::unnamed"
  skip_tests:
    type: boolean
    required: false
    description: "Skip tests directory (default: false)"
    default: false
  skip_eject:
    type: boolean
    required: false
    description: "Keep templates/ directory (default: false)"
    default: false
outputs:
  working_dir:
    type: string
    description: "Path to the new project directory"
  status:
    type: string
    enum: [ready, failed]
---

# Skill: ai-warevo-cpp-init

## Purpose
Create a new C++23 project using the `template_cpp` generator (like `dotnet new`). Supports `cli`, `static`, and `dynamic` library templates.

## Input Validation
1. Check that `project_name` matches `^[a-z][a-z0-9_-]*$`
2. Verify `project_type` is one of: `cli`, `static`, `dynamic`
3. If `namespace` is provided, validate it contains only `::`-separated identifiers
4. If validation fails, report error and stop

## Inputs
- Project name (e.g., `my-lib`)
- Project type: `cli`, `static`, or `dynamic`
- Namespace (optional, default: `aw::unnamed`)
- Skip tests (optional, default: false)
- Skip eject (optional, default: false)

## Outputs
- New project directory with CMakeLists.txt, src/, include/, tests/
- Git repository ready for commits

## Workflow

### Step 1: Locate template_cpp repository

```bash
TEMPLATE_DIR="${TEMPLATE_CPP_PATH:-$HOME/workspace/template_cpp}"

if [ ! -d "$TEMPLATE_DIR" ]; then
  echo "error: template_cpp not found at $TEMPLATE_DIR" >&2
  echo "Please clone it first: git clone https://github.com/ai-warevo/template_cpp.git $TEMPLATE_DIR" >&2
  exit 1
fi

# Verify it's the correct repository
if [ ! -f "$TEMPLATE_DIR/templates/scaffold.sh" ]; then
  echo "error: $TEMPLATE_DIR doesn't look like template_cpp (scaffold.sh not found)" >&2
  exit 1
fi
```

**Key Change**: Require template_cpp to exist instead of cloning it. This avoids network issues and ensures a specific, known version.

### Step 2: Validate inputs

```bash
PROJECT_NAME="<project_name>"
PROJECT_TYPE="<project_type>"
NAMESPACE="<namespace>"

# Validate project name
if ! echo "$PROJECT_NAME" | grep -qE '^[a-z][a-z0-9_-]*$'; then
  echo "error: project name must match ^[a-z][a-z0-9_-]*$ (got: $PROJECT_NAME)" >&2
  exit 1
fi

# Validate project type
case "$PROJECT_TYPE" in
  cli|static|dynamic) ;;
  *)
    echo "error: project_type must be cli, static, or dynamic (got: $PROJECT_TYPE)" >&2
    exit 1
    ;;
esac

# Validate namespace (basic check)
if ! echo "$NAMESPACE" | grep -qE '^[a-zA-Z][a-zA-Z0-9_]*(::[a-zA-Z][a-zA-Z0-9_]*)*$'; then
  echo "error: namespace must be valid C++ identifiers separated by :: (got: $NAMESPACE)" >&2
  exit 1
fi
```

### Step 3: Determine output directory

```bash
# ALWAYS generate to explicit output directory (never in-place in template_cpp)
PROJECT_DIR="${PROJECT_DIR:-$HOME/workspace/<project_name>}"

# Create output directory
mkdir -p "$PROJECT_DIR"

if [ "$(ls -A "$PROJECT_DIR")" ]; then
  echo "error: output directory not empty: $PROJECT_DIR" >&2
  exit 1
fi
```

**Key Change**: Always use explicit `OUT=` parameter. Never generate in template_cpp directory itself.

### Step 4: Run generator with output directory parameter

```bash
NS="<namespace>"
TESTS_OPT=""
[ "<skip_tests>" = "true" ] && TESTS_OPT="TESTS=0"

# Use OUT= parameter to generate directly to target directory
# This avoids manual file movement and "make eject" hassles
make -C "$TEMPLATE_DIR" init \
  TYPE="<project_type>" \
  NAME="<project_name>" \
  NS="$NS" \
  OUT="$PROJECT_DIR" \
  $TESTS_OPT

if [ $? -ne 0 ]; then
  echo "error: template_cpp init failed" >&2
  exit 1
fi
```

**Key Change**: Use `OUT=` parameter instead of generating in-place and moving files.

### Step 5: Fix common CMakeLists.txt issues

```bash
cd "$PROJECT_DIR"

# If using FetchContent for spdlog/json, increase timeout to avoid network issues
if grep -q "FetchContent_Declare" CMakeLists.txt; then
  sed -i 's/TIMEOUT 60/TIMEOUT 180/g' CMakeLists.txt || true
  
  # For json, prefer direct download over git clone (faster, more reliable)
  if grep -q "nlohmann_json.*git@github.com" CMakeLists.txt; then
    cat >> CMakeLists.txt << 'EOF'

# Note: Consider using direct download instead for faster builds:
# FetchContent_Declare(
#     nlohmann_json
#     URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
#     DOWNLOAD_NO_EXTRACT ON
#     TIMEOUT 60)
EOF
  fi
fi
```

### Step 6: Verify configuration files are present

```bash
# Config files should have been copied by the generator
for config_file in .clang-format .clang-tidy .editorconfig; do
  if [ ! -f "$config_file" ]; then
    echo "warning: $config_file not found, copying from template_cpp" >&2
    cp "$TEMPLATE_DIR/$config_file" . 2>/dev/null || true
  fi
done

# Ensure .gitignore doesn't incorrectly exclude config files
if grep -q '\.clang-tidy\|\.clang-format' .gitignore 2>/dev/null; then
  echo "warning: .gitignore incorrectly excludes .clang-tidy or .clang-format" >&2
  echo "         These are version-controlled config files, not build artifacts" >&2
  sed -i '/\.clang-tidy/d; /\.clang-format/d' .gitignore
fi
```

### Step 7: Initialize git repository

```bash
cd "$PROJECT_DIR"

if [ ! -d .git ]; then
  git init
  git add .
  git commit -m "chore: init C++23 project from template_cpp

Generated with:
  - Type: <project_type>
  - Namespace: <namespace>
  - CMake: $(cmake --version | head -1)
  - Compiler: $(clang++ --version 2>/dev/null | head -1 || g++ --version | head -1)"
fi
```

### Step 8: Verify build works

```bash
# Quick verification that generated project can build
if ! cmake --preset debug -B build/debug 2>&1 | head -5; then
  echo "warning: initial cmake configuration failed" >&2
  echo "         This may be due to missing dependencies or network issues" >&2
  echo "         See TROUBLESHOOTING.md for solutions" >&2
fi
```

### Step 9: Return result

Print the project path with status:

```
✅ Project created: $PROJECT_DIR
   Type:        <project_type>
   Namespace:   <namespace>
   Tests:       $([ "<skip_tests>" = "true" ] && echo "disabled" || echo "enabled")
   
Next steps:
  cd $PROJECT_DIR
  make build
  make test
  
See template_cpp/KNOWN_ISSUES_AND_SOLUTIONS.md for troubleshooting FetchContent timeouts
```

## Rules
- Project name must be lowercase alphanumeric with underscores/hyphens
- Project name must not be a C++ keyword and must not start with `__`
- Namespace uses `::` separator (e.g., `aw::foo::bar`)
- If `make init` fails, do not proceed to git init
- Generator is pure Bash (3.2+), no Python required
- Templates are C++23 with `-Wall -Wextra -Wpedantic` (or `/W4` on MSVC)
- All templates include Catch2 v3 tests (unless skipped)

## Template Details

| TYPE | Aliases | Output |
|------|---------|--------|
| `cli` | `console`, `exe`, `app` | Executable with core library + `main.cpp` |
| `static` | `lib`, `library` | Static library with public header |
| `dynamic` | `dll`, `so`, `shared` | Shared library with `GenerateExportHeader` macros |

## Dependencies
- `template_cpp` repository (pre-cloned to `$HOME/workspace/template_cpp` or `$TEMPLATE_CPP_PATH`)
- `cmake` 3.23+
- C++23 compiler (Clang 18+ or MSVC 2022+)
- `make` or `bash` (generator is pure Bash 3.2+)

---

## Common Issues and Solutions

### Issue: FetchContent Timeout for nlohmann/json or spdlog

**Symptom**:
```
CMake Error: Failed to clone repository: 'https://github.com/nlohmann/json.git'
CMake Error: Head sha can't be blank
```

**Root Cause**: Network latency or repository access issues during `FetchContent_MakeAvailable()`.

**Solutions** (in order of preference):

#### Solution 1: Use pre-downloaded headers for json (RECOMMENDED)
```cmake
# In generated CMakeLists.txt, replace FetchContent for json with:
add_library(nlohmann_json INTERFACE)
target_include_directories(nlohmann_json INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann)

# Then download once:
# mkdir -p external/nlohmann
# wget -q https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp \
#   -O external/nlohmann/json.hpp
```

#### Solution 2: Increase FetchContent timeout
```cmake
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
    GIT_SHALLOW ON
    TIMEOUT 180)  # Increased from 60 to 180 seconds
```

#### Solution 3: Use system packages
```bash
# Ubuntu/Debian
sudo apt-get install nlohmann-json3-dev libspdlog-dev catch2

# Then use find_package() instead of FetchContent
```

#### Solution 4: Set environment variables for CMake
```bash
export CMAKE_PREFIX_PATH=/usr/local/lib/cmake
cmake --preset debug
```

**Prevention**: The skill automatically increases timeout to 180 seconds in generated projects.

---

### Issue: .gitignore incorrectly excludes config files

**Symptom**: `.clang-tidy` and `.clang-format` are missing after git clone (not ignored properly).

**Root Cause**: Generated `.gitignore` incorrectly lists `.clang-tidy` and `.clang-format` as build artifacts.

**Solution**: These are version-controlled config files:
```bash
# CORRECT .gitignore pattern:
build/                    # Build directory
_deps/                    # CMake dependencies
.clangd/                  # Clang tools cache
*.o *.a *.so              # Object files
CMakeCache.txt            # CMake cache

# DO NOT GITIGNORE (they are config files):
# .clang-tidy              <-- This is config!
# .clang-format            <-- This is config!
# .editorconfig            <-- This is config!
```

**Prevention**: The skill verifies and fixes `.gitignore` after generation.

---

### Issue: CMakeLists.txt auto-discovery of sources fails

**Symptom**: Linker error `undefined reference to ...` even though source files exist.

**Root Cause**: `aw_core_library()` macro's implicit source file discovery doesn't find files matching the pattern.

**Solution**: Make sources explicit in CMakeLists.txt:
```cmake
# BAD - implicit discovery:
aw_core_library(NAME mylib ID mylib)

# GOOD - explicit sources:
aw_core_library(
    NAME mylib
    ID mylib
    SOURCES
        src/main.cpp
        src/helper.cpp)

target_link_libraries(mylib PUBLIC nlohmann_json spdlog::spdlog)
```

**Prevention**: Generated projects will include explicit source lists by default.

---

### Issue: Build preset not available on current platform

**Symptom**: `cmake: unknown preset 'clang18-libcxx'` on Windows.

**Root Cause**: CMakePresets.json includes platform-specific presets without conditions.

**Solution**: Use platform-agnostic preset or specify explicitly:
```bash
cmake --preset default          # Works everywhere
cmake --preset clang18-libcxx   # Linux/macOS only
cmake --preset msvc             # Windows only
```

Or let CMake choose:
```bash
cmake -B build
cmake --build build
```

**Prevention**: Generated CMakePresets.json will include platform detection conditions.

---

### Issue: Command not found during build

**Symptom**:
```
sh: clang++-18: command not found
sh: make: command not found
```

**Solution**: Install required tools:
```bash
# Ubuntu/Debian
sudo apt-get install clang-18 clang-tidy build-essential cmake

# macOS
brew install llvm cmake

# Windows
# Install Visual Studio 2022 or standalone MSVC build tools
```

---

## Best Practices

### ✅ DO:
- Use `OUT=` parameter to generate to separate directory
- Let the skill handle FetchContent timeout increases
- Keep `.clang-format`, `.clang-tidy`, `.editorconfig` in version control
- Make sources explicit in CMakeLists.txt
- Run `make lint` regularly to catch issues early
- Build with `make build` before `make test`

### ❌ DON'T:
- Generate projects in-place in template_cpp directory
- Gitignore `.clang-tidy`, `.clang-format`, or `.editorconfig`
- Rely on implicit source file discovery
- Run `make eject` unless specifically needed
- Delete generated CMakePresets.json (use it to build)

---

## Troubleshooting Checklist

Before reporting issues:
- [ ] `cmake --version` shows 3.23 or later
- [ ] Compiler is available: `clang++ --version` or `g++ --version`
- [ ] `make` is available: `make --version`
- [ ] Template_cpp exists: `ls $HOME/workspace/template_cpp/templates/scaffold.sh`
- [ ] Network access works: `ping github.com`
- [ ] `.gitignore` doesn't exclude `.clang-tidy` or `.clang-format`
- [ ] CMakeLists.txt uses explicit source lists
- [ ] Tried `rm -rf build && cmake --preset default` to rebuild from scratch

---

## Related Documentation

- [template_cpp README](https://github.com/ai-warevo/template_cpp/README.md)
- [template_cpp KNOWN_ISSUES_AND_SOLUTIONS.md](https://github.com/ai-warevo/template_cpp/KNOWN_ISSUES_AND_SOLUTIONS.md)
- [AI-Warevo C++ Best Practices](https://github.com/ai-warevo/.github/docs/cpp-best-practices.md)
