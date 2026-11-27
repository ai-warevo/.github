# CMake Build Action

A composite GitHub Action for configuring, building, and optionally installing a CMake project.

## Purpose

Wraps the standard CMake configure → build → install lifecycle into a single reusable step. Supports multi-config generators (MSVC) via the `config` input and optional `cmake --install`.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `source_dir` | Source directory (relative to workspace) | No | `.` |
| `build_dir` | Build directory (relative to `source_dir`) | No | `build` |
| `build_type` | `CMAKE_BUILD_TYPE` (`Debug`, `Release`, `RelWithDebInfo`, `MinSizeRel`) | No | `Release` |
| `extra_args` | Additional CMake configure arguments (space-separated) | No | — |
| `parallel` | Use `--parallel` for build | No | `true` |
| `config` | Build config for multi-config generators (e.g. `Release` for MSVC) | No | Auto-detect from `build_type` |
| `install` | Run `cmake --install` after build | No | `false` |

## Usage example

```yaml
# Basic configure and build
- name: Configure & Build
  uses: ai-warevo/<repo>/.github/actions/cmake-build@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    source_dir: .
    build_dir: build
    build_type: Debug
    extra_args: -DAW_TPL_BUILD_TESTS=ON

# With toolchain args from setup-toolchain
- uses: ai-warevo/<repo>/.github/actions/cmake-build@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    extra_args: "${{ steps.tc.outputs.cmake_args }} -DAW_TPL_WARNINGS_AS_ERRORS=ON"
```
