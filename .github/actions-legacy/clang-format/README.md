# Clang-Format Check Action

A composite GitHub Action for checking C++ formatting with `clang-format`, failing if any file is misformatted.

## Purpose

Runs `clang-format --dry-run --Werror` on all C++ source and header files. Fails the step if any file is not properly formatted (CI breaks on format violations). Requires `clang-format` to be installed (e.g. `clang-format-18` in the `ci-cpp23` container).

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `source_dir` | Source directory to check (relative to workspace) | No | `.` |
| `extensions` | File extensions to check, comma-separated | No | `cpp,hpp` |
| `config_file` | `clang-format` config file path | No | `.clang-format` |
| `exclude` | Exclude patterns (comma-separated, passed to `find -prune`) | No | `_deps/*,build/*,CMakeFiles/*,cmake/*` |

## Usage example

```yaml
- name: Check formatting
  uses: ai-warevo/<repo>/.github/actions/clang-format@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    source_dir: .
    extensions: cpp,hpp
    config_file: .clang-format
    exclude: _deps/*,build/*
```
