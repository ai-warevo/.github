# GCOVR Coverage Report Action

A composite GitHub Action for generating coverage reports (XML + HTML) from a CMake build.

## Purpose

Generates both XML (for parsing) and HTML (for deployment) coverage reports using `gcovr`. Automatically pins `gcov` to match the compiler version. Requires `gcovr` and `gcov` to be installed (e.g. in the `ci-cpp23` container).

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `source_dir` | Source directory (relative to workspace) | No | `.` |
| `build_dir` | Build directory (relative to `source_dir`) | No | `build` |
| `source_filter` | `--filter` patterns (space-separated path prefixes to match) | No | — |
| `exclude` | `--exclude` pattern (comma-separated) | No | `_deps/*` |
| `output_dir` | Output directory for HTML report | No | `htmlcov` |
| `xml_path` | Output path for `coverage.xml` | No | `coverage.xml` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `xml_path` | Path to generated `coverage.xml` | `coverage.xml` |
| `html_dir` | Path to generated HTML report directory | `htmlcov` |
| `percent` | Line coverage percentage | `73` |

## Usage example

```yaml
- name: Generate Coverage
  id: coverage
  uses: ai-warevo/<repo>/.github/actions/gcovr-report@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    source_dir: .
    build_dir: build
    source_filter: include/aw/tpl/ src/
    exclude: _deps/*
    output_dir: htmlcov
```
