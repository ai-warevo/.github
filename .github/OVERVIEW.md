# CI Infrastructure

This repository hosts reusable GitHub Actions workflows and composite actions for `ai-warevo` projects. The active CI implementation is maintained in `.github/workflows/` and `.github/actions/`; the previous implementation remains preserved in `.github/workflows-legacy/` and `.github/actions-legacy/` for reference.

## Active workflows

| Workflow | Purpose |
| --- | --- |
| `on_pr__main.yml` | Pull request build/test and coverage checks for the C++23 project in `example/` |
| `on_push__main.yml` | Main-branch build/test and coverage deployment for `example/` |
| `on_wfd__release.yml` | Resolve release version, build/test `example/`, run sanitizers, package artifacts, and publish a GitHub release |
| `on_wfd__actions_test.yml` | Manually run the composite-action test workflows |
| `reusable__*.yml` | Reusable C++23 build, coverage, sanitizer, and release workflows |
| `test__*.yml` | Test workflows for the reusable composite actions |

The three event wrappers invoke the reusable workflows in this checkout so changes are validated before publication. Consumer repositories should call the published workflows and actions using `ai-warevo/.github` references.

## C++23 example target

The repository's active C++ CI targets `example/`, not the infrastructure repository root. This project uses CMake and Catch2 v3, with the following switches used by CI:

- `AW_EXAMPLE_BUILD_TESTS=ON` enables the example test suite.
- `AW_EXAMPLE_ENABLE_COVERAGE=ON` enables coverage instrumentation.
- `AW_EXAMPLE_SANITIZE=address|thread` enables separate sanitizer configurations.

The coverage job filters to example source, test, and public include directories and excludes FetchContent dependencies. The release workflow packages the built host and plugin artifacts. There is no separate active `example-ci.yml`; PR, push, and manual release validation use the `__lox` workflow set.

## Reusable workflow contracts

| Workflow | Purpose |
| --- | --- |
| `reusable__common__build_cpp23.yml` | Compiler matrix build and tests; supports caller-provided commands and optional artifact upload |
| `reusable__common__coverage_cpp23.yml` | Coverage generation, parsing, threshold check, and optional badge/pages deployment |
| `reusable__common__sanitizers_cpp23.yml` | ASan/UBSan and TSan build/test matrix |
| `reusable__release__resolve_version.yml` | Resolve a version and release notes from the root `VERSION`, tags, or supported manifest |
| `reusable__release__publish.yml` | Update release metadata, checksum artifacts, create tag, and publish GitHub release |

Reusable workflows are the source of truth. Consumer workflows can call them from `ai-warevo/.github/.github/workflows/<workflow>@<ref>` and pass project-specific commands where needed. Existing root-level C++ consumers can use the defaults; projects with a nested source tree should provide command inputs or a working directory.

## Composite actions

The active action catalog consists of:

| Action | Purpose |
| --- | --- |
| `common__deploy_to_branch_io` | Deploy files to a branch, including GitHub Pages or badge branches |
| `common__parse_coverage_pure` | Parse coverage XML and return percentage/color metrics |
| `common__render_svg_badge_pure` | Render an SVG badge from supplied labels and colors |
| `release__checksums_io` / `release__checksums_pure` | Generate and verify SHA-256 checksum files |
| `release__resolve_next_release_io` / `release__resolve_next_release_pure` | Resolve the next version and release notes |
| `release__write_release_files_io` / `release__write_release_files_pure` | Update `VERSION` and `CHANGELOG.md` for a release |

Composite actions are referenced by consumers as `ai-warevo/.github/.github/actions/<action>@<ref>`. The `__lox` implementation uses Python standard library for pure computation and composite wrappers for GitHub I/O.

## Permissions and supply chain

Reusable workflows should declare only the permissions they need. Consumer repositories may pin `@main` references to a reviewed commit SHA for stricter supply-chain control. The in-repository wrappers use local reusable workflow paths so they test the current checkout.
