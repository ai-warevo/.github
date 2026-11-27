# Setup Toolchain Action

A composite GitHub Action for computing CMake arguments for a given toolchain configuration.

## Purpose

Sets up a C++ toolchain (compiler, standard library, sanitizer) and emits CMake arguments compatible with [cmake-build](.github/actions/cmake-build/README.md). Supports two execution modes:

- **Container mode** (`container_mode: true`): skips installation, only emits CMake arguments. Used in custom containers (e.g. `ghcr.io/ai-warevo/ci-cpp23`) where compilers are pre-installed.
- **Native mode** (default on GitHub-hosted runners): installs the toolchain via `apt-get` or MSVC Dev Cmd.

Supported toolchains: `clang18-libcxx`, `clang18-libstdcxx`, `gcc15`, `msvc`.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `toolchain` | `clang18-libcxx` \| `clang18-libstdcxx` \| `gcc15` \| `msvc` | Yes | — |
| `container_mode` | Skip installation, only compute CMake args | No | Auto-detected (no `sudo` = container) |
| `sanitizer` | Sanitizer: `address`, `thread`, `memory`, `undefined` | No | None |
| `sanitizer_runtime` | Install clang sanitizer runtimes (`libclang-rt-18-dev`) | No | `false` |
| `tools` | Also install `clang-format-18` / `clang-tidy-18` | No | `false` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `cmake_args` | Arguments for `cmake-build` `extra_args` (no spaces inside a single argument) | `-GNinja -DCMAKE_CXX_COMPILER=clang++-18 -DCMAKE_CXX_FLAGS=-stdlib=libc++ -fexperimental-library -DCMAKE_EXE_LINKER_FLAGS=-stdlib=libc++ -DCMAKE_SHARED_LINKER_FLAGS=-stdlib=libc++` |
| `compiler` | C++ compiler name | `clang++-18` |
| `stdlib` | Standard library | `libc++` |

## Usage example

### Container mode

```yaml
- id: tc
  uses: ai-warevo/<repo>/.github/actions/setup-toolchain@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    toolchain: clang18-libcxx
    container_mode: true
    sanitizer: address
- uses: ai-warevo/<repo>/.github/actions/cmake-build@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    extra_args: "${{ steps.tc.outputs.cmake_args }}"
```

### Native mode

```yaml
- id: tc
  uses: ai-warevo/<repo>/.github/actions/setup-toolchain@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    toolchain: clang18-libcxx
    container_mode: false
    sanitizer_runtime: true
    tools: true
```
