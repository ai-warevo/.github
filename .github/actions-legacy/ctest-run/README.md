# CTest Run Action

A composite GitHub Action for running CTest for a CMake project.

## Purpose

Wraps `ctest` execution with configurable build config, output options, and test timeout. Designed to run after [cmake-build](.github/actions/cmake-build/README.md).

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `source_dir` | Source directory (relative to workspace) | No | `.` |
| `build_dir` | Build directory (relative to `source_dir`) | No | `build` |
| `config` | CMake build configuration (`Debug`, `Release`, etc.) | No | Auto-detect |
| `output_on_failure` | Show output only on failure | No | `true` |
| `timeout` | Test timeout in seconds | No | None |

## Usage example

```yaml
- name: Run Tests
  uses: ai-warevo/<repo>/.github/actions/ctest-run@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    source_dir: example
    build_dir: build
    config: Debug
    timeout: 120
```
