# Changelog

All notable changes to this project are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<!--
Maintainer notes:
- Add every user-visible change under `## [Unreleased]` in the same commit that makes it.
- Categories: Breaking, Added, Changed, Deprecated, Removed, Fixed, Security. Omit empty ones.
- scripts/release.sh / scripts/release.ps1 derive the next version from those categories.
-->

## [Unreleased]

## [1.8.4] - 2026-11-22

### Changed

- Merge branch 'main' of github.com:ai-warevo/.github
- ci <3

## [1.8.3] - 2026-11-22

### Changed

- ci <3

## [1.8.2] - 2026-11-22

### Changed

- Merge pull request #50 from ai-warevo/loktionov129-patch-6
- Update README.md
- Merge branch 'main' of github.com:ai-warevo/.github
- clang-tidy

## [1.8.1] - 2026-11-22

### Changed

- clang-tidy
- fix
- finalize canonical clang-tidy configuration with all exclusions
- merge project-specific clang-tidy exclusions into canonical config
- use tobus .clang-tidy as canonical configuration
- synchronize clang-tidy config across all projects
- Merge branch 'main' of github.com:ai-warevo/.github

### Fixed

- suppress modernize-use-nodiscard for Catch2
- check existing entries before early exit
- suppress Catch2 framework warnings in clang-tidy

## [1.8.0] - 2026-11-21

### Added

- add setup_command parameter to reusable workflows

### Changed

- ci
- concurrency
- Merge pull request #45 from ai-vibeqodez/fix/sanitizers-libstdcxx

### Fixed

- fixes
- ASan+UBSan job — use clang18-libstdcxx instead of libc++

## [1.7.0] - 2026-11-21

### Added

- add Windows (MSVC) support to cmake-build and ctest-run actions

### Changed

- Merge pull request #44 from ai-warevo/loktionov129-patch-1
- Update README.md
- ci
- ci
- clang-tidy
- Merge pull request #43 from ai-vibeqodez/feat/windows-actions
- Merge pull request #42 from ai-vibeqodez/feat/windows-actions
- Merge pull request #41 from ai-vibeqodez/feat/windows-actions
- Merge pull request #40 from ai-warevo/feat/windows-actions

### Fixed

- preserve untracked files in badge-deploy
- remove untracked htmlcov before branch checkout
- fetch remote branch before checkout
- use matching gcov version explicitly
- preserve coverage HTML during gh-pages checkout
- ci

## [1.6.5] - 2026-11-20

### Changed

- split reusable release responsibilities (#39)

## [1.6.4] - 2026-11-20

### Changed

- extract TSan setup into dedicated action
- fix
- fix
- Merge pull request #38 from ai-vibeqodez/fix/clang-tidy-config
- Merge branch 'main' of github.com:ai-warevo/.github

### Fixed

- one more ime
- ci

## [1.6.3] - 2026-11-20

### Changed

- fix
- agent
- Merge branch 'main' of github.com:ai-warevo/.github
- c++23

## [1.6.2] - 2026-11-20

### Changed

- fixes uses
- fixes uses
- fixes uses
- fix
- fixxxx
- fixiki
- fixes
- fixes
- fix
- fix
- fix
- fix'
- fix

### Fixed

- ci
- ci

## [1.6.1] - 2026-11-20

### Changed

- fix
- fix release
- pin all action references to @main
- update action SHA pinning to latest commit (main)
- Merge branch 'fix/ci-sha-pins' of github.com:ai-vibeqodez/.github
- clean up ci/.github/workflows, SHA-pin template wrappers
- Merge pull request #33 from ai-vibeqodez/refactor/remove-scripts
- rename workflows to pr-fast / weekly-deep / release model
- remove scripts/, replace with CI composite actions
- Merge pull request #30 from ai-vibeqodez/fix/issue-27

### Fixed

- changes
- release
- resolve merge conflicts — use @main for ai-warevo actions
- 42
- docs
- skip Conventional Commits validation for merge commits
- update composite action SHAs and fix paths
- update composite action SHAs and fix paths in workflows
- merge pr
- use relative paths for composite actions in reusable workflows
- correct action paths and update SHAs in template workflows
- fix
- add missing release.yml universal template (#27)
- scripts: update

## [1.6.0] - 2026-11-19

### Added

- Add `CONTRIBUTING.md` with development setup, commit conventions, PR process, and guidelines for adding CI templates, composite actions, and skills (#5)
- Add `docs/ARCHITECTURE.md` with full architecture overview: CI/CD tiers, composite actions, workflow hierarchy, release pipeline, AI skills pipeline, and data flow diagrams (#5)
- Add language-agnostic Makefile-contract CI workflows, caller templates, and `ci-setup` agent skill
- Expand `templates/changelog_template.md`: add versioning guide, example entries for all categories, and writing guidelines (#5)
- Expand `templates/contributing_template.md`: add development setup, build/test instructions, commit conventions, and PR process (#5)
- Expand `templates/overview.md`: add task plan table, progress metrics, risks table, and decisions log (#5)
- Expand `templates/readme_template.md`: add badges, quick start, structure, API reference, and testing sections
- Expand `templates/task_template.md`: add implementation checklist, acceptance criteria, and result section with examples (#5)

### Security

- Guard placeholder steps in universal CI templates (`ci.yml`, `test.yml`) with `if: false` to prevent accidental execution when no language stack is configured
- Harden `docker-compose.yml`: remove hardcoded API key default, add resource limits, configurable host port
- Replace all `@main` action pinning with 40-char commit SHA hashes across all workflow and action files; resolves supply-chain risk from compromised branch refs (#1)

## [1.5.0] - 2026-11-18

### Added

- `scripts/updater.sh` — automated script sync from remote `ai-warevo/.github`; HTTP version check, consent prompts, conflict detection
- `scripts/updater.ps1` — PowerShell wrapper for `updater.sh` (Windows)
- `make update` target — cross-platform script sync (bash on Linux/macOS, PowerShell on Windows)
- `SCRIPT_VERSION` metadata in all scripts for version tracking
- Pre-flight update checks in `changelog-update.sh` and `release.sh`

### Fixed

- Makefile `ifneq`/`ifeq` tab indentation in `changelog-dry-run` and `update` targets

## [1.4.8] - 2026-11-18

### Added

- example CI: upload badge and coverage HTML as artifacts, add deploy job for GitHub Pages
- example CI: add `--security-opt seccomp=unconfined` to sanitizer container for TSan compatibility

### Fixed

- coverage-html-deploy: use `.` as default target_folder, create folder before `.nojekyll`, tolerate `git rm -rf` on new branch, copy dotfiles correctly
- badge: resolve output path with `$(pwd)` instead of `${{ github.workspace }}`
- actions-test: rename workflow to lowercase, simplify Docker context for docker-build-push, remove verbose debug steps
- example tests: use `PRE_TEST` discovery mode to avoid TSan + Docker ASLR/seccomp conflict (exit 66)

## [1.4.7] - 2026-11-18

### Fixed

- fix sysctl and badge-deploy ordering issues

## [1.4.6] - 2026-11-18

### Fixed

- badge-deploy: preserve `.git` directory when cleaning orphan branch (fix `fatal: not in a git directory`)

## [1.4.5] - 2026-11-18

### Fixed

- split CXX flags to avoid shell-split in cmake_args
- resolve badge path relative to github.workspace in badge-deploy

## [1.4.4] - 2026-11-18

### Fixed

- run C++ jobs in ci-cpp23 Docker container

## [1.4.3] - 2026-11-18

### Fixed

- fix duplicate heading and empty tag list handling

## [1.4.2] - 2026-11-18

### Fixed

- scripts
- remove github.token template from input description
- composite action token input ΓÇö use github.token fallback in env block

## [1.4.1] - 2026-11-18

### Fixed

- cmake-build extra_args quoting and setup-toolchain flag generation
- quote CMake flag values with spaces in setup-toolchain action
- fix
- retry loop with dpkg fix for transient PPA 503 errors

### Changed

- add container_mode support to composite actions
- add MSVC support to container CI templates
- Update README.md

## [1.4.0] - 2026-11-17

### Added

- add msvc toolchain option to setup-toolchain
- clang-linter

### Fixed

- dsh
- linter fix
- cmake

### Changed

- fix clang-tidy

## [1.3.0] - 2026-11-16

### Added

- add ci-infrastructure-integrator skill

## [1.2.16] - 2026-11-16

### Fixed

- use loaded image name instead of hardcoded 'image'
- add 'artifact_path' input for flexible tar filename
- add 'load' input to control local daemon load
- always load image to local daemon
- load image locally when push=false for testing
- edge-cases default_badge uses workspace-relative path
- docker-build-push match release.yml context/dockerfile pattern
- docker-build-push uses example/ as build context

## [1.2.15] - 2026-11-16

### Fixed

- docker-save tests pull alpine:3.19 before saving
- edge-cases default_badge creates output directory
- edge-cases missing_tag expects changelog-release failure
- checksums-verify test creates test files and SHA256SUMS.txt
- changelog-release missing tag test expects action failure

### Changed

- update pre-commit hook instructions in README

## [1.2.14] - 2026-11-16

### Fixed

- fix coverage badge — gcov version mismatch and parser formula
- remove duplicate @main in uses references

## [1.2.13] - 2026-11-16

### Fixed

- fix logical errors and standardize @main references

## [1.2.12] - 2026-11-16

### Fixed

- remove event condition from coverage steps
- add gcovr-report action to example-ci paths filter
- add gcovr-report action to example-ci paths filter

### Changed

- Revert "fix(ci): add gcovr-report action to example-ci paths filter"
- Revert "fix(ci): add gcovr-report action to example-ci paths filter"

## [1.2.11] - 2026-11-16

### Fixed

- use build dir as -r and absolute paths in filters

## [1.2.10] - 2026-11-16

### Fixed

- gcovr

## [1.2.9] - 2026-11-16

### Changed

- Reapply "fix: use --verbose=2 instead of -v 2 to avoid arg parsing issue"

## [1.2.8] - 2026-11-16

### Fixed

- use --verbose=2 instead of -v 2 to avoid arg parsing issue

### Changed

- Revert "fix: use --verbose=2 instead of -v 2 to avoid arg parsing issue"

## [1.2.7] - 2026-11-16

### Fixed

- add verbose gcovr logging for coverage debugging
- gcovr source root must be build dir, filters match relative paths

## [1.2.6] - 2026-11-16

### Fixed

- gcovr source root must be source dir, not build dir

## [1.2.5] - 2026-11-16

### Fixed

- gcovr filter patterns to match tpl workflow approach

## [1.2.4] - 2026-11-16

### Fixed

- gcovr-report: install via pip (not apt), add set -euo pipefail, support multiple --filter patterns, add --gcov-ignore-parse-errors
- example-ci: use explicit source_root and filter patterns (plugin/ host/ plugin/tests/) for reliable coverage data collection

## [1.2.3] - 2026-11-16

### Fixed

- match CHANGELOG.md/VERSION regardless of status prefix
- allow unstaged changes to CHANGELOG.md and VERSION
- replace xmllint with grep in coverage-parser action

## [1.2.2] - 2026-11-16

### Fixed

- apply coverage/sanitizer flags via target_compile_options

### Changed

- add AGENTS.md guide and update README files

## [1.2.1] - 2026-11-16

### Changed

- move commit validation to commit-msg hook, fix PowerShell 5.1 compatibility

## [1.2.0] - 2026-11-15

### Added

- add 'release' type to commit message hook
- add changelog and changelog-dry-run make targets

### Fixed

- use Conventional Commits format for release commit message
- remove 'Other' category, map to 'Changed' for release compatibility

### Changed

- use prepare-commit-msg instead of pre-commit in README instructions
- add prepare-commit-msg hook for Conventional Commits validation
- add pre-commit hook for Conventional Commits validation
- add changelog-update scripts for auto-generating [Unreleased] from git log
- add .gitattributes rule for shell script LF line endings
- add 3 bug fixes to Unreleased
- fix gcovr source_root, coverage-parser xmllint, docker-buildx v4
- remove debug logging from release workflow

## [1.1.0] - 2026-11-15

### Added

- add 'release' type to commit message hook
- add changelog and changelog-dry-run make targets

### Fixed

- remove 'Other' category, map to 'Changed' for release compatibility

### Changed

- use prepare-commit-msg instead of pre-commit in README instructions
- add prepare-commit-msg hook for Conventional Commits validation
- add pre-commit hook for Conventional Commits validation
- add changelog-update scripts for auto-generating [Unreleased] from git log
- add .gitattributes rule for shell script LF line endings
- add 3 bug fixes to Unreleased
- fix gcovr source_root, coverage-parser xmllint, docker-buildx v4
- remove debug logging from release workflow

## [1.0.6] - 2026-11-15

### Fixed

- `release.yml`: fix libplugin.so upload path to `example/build/plugin/libplugin.so`

## [1.0.5] - 2026-11-15

### Fixed

- `release.yml`: add build output listing before artifact upload to debug missing libplugin.so

## [1.0.4] - 2026-11-15

### Fixed

- `release.yml`: replace gh CLI with curl in debug logging (gh not available without GH_TOKEN in workflows)

## [1.0.3] - 2026-11-15

### Fixed

- `release.yml`: add debug logging for artifact download troubleshooting (list available artifacts, ls -laR before/after download)

## [1.0.2] - 2026-11-15

### Fixed

- `release.yml`: download artifacts by exact name into shared dir, reference correct filenames (libplugin.so, host) instead of pattern-based download with mismatched names
- `example-ci.yml`: correct coverage-parser path, use published `ai-warevo/.github/.github/actions/coverage-parser@main`

## [1.0.1] - 2026-11-15

### Fixed

- `ctest-run`: accept `source_dir` input, cd to build directory before running ctest (was failing when source_dir ≠ `.`)
- `gcovr-report`: accept `source_dir` input, resolve source_root correctly for non-root source directories
- `release.yml`: remove duplicate `v` prefix from `tag`, `tag_name`, and `name` inputs (release-version already returns version with `v`)

## [1.0.0] - 2026-11-15

### Added

- Composite action `cmake-build`: CMake configure + build wrapper
- Composite action `ctest-run`: CTest execution wrapper
- Composite action `gcovr-report`: gcovr install + coverage report generation (XML + HTML)
- Composite action `badge-deploy`: SVG badge isolation + GitHub Pages deploy
- Composite action `coverage-html-deploy`: Coverage HTML deploy to gh-pages
- Composite action `docker-build-push`: Docker Buildx + login + build-push pipeline

### Changed

- `example-ci.yml`: simplified from 135 to 95 lines using new composite actions
- `release.yml`: simplified from 156 to 148 lines using `docker-build-push`
- `test.yml`: simplified using `badge-deploy` and `coverage-html-deploy`, added C++ stubs
- `ci.yml`: C++ stubs updated to reference `cmake-build` and `ctest-run`

## [0.7.10] - 2026-11-15

### Fixed

- Release workflow: checkout main instead of tag so changelog-release can find release notes

## [0.7.9] - 2026-11-15

### Fixed

- Changelog release action: add release notes for v0.7.8

## [0.7.8] - 2026-11-15

### Fixed

- Release workflow: build artifacts before creating release so they attach to GitHub Release

## [0.7.7] - 2026-11-15

### Changed

- Docker build: remove debug step, keep only build-and-push job

## [0.7.6] - 2026-11-15

### Fixed

- Docker build: use `context: ./example` with `file: example/Dockerfile` (context scoped to example/, Dockerfile path relative to repo root)

## [0.7.5] - 2026-11-15

### Fixed

- Docker build: fix `file` path in `build-push-action` to `example/Dockerfile` with repository root context

## [0.7.4] - 2026-11-15

### Fixed

- Docker build: fix `context` and `file` paths in `build-push-action` to use `./example` context with correct Dockerfile reference
- Docker build: add debug step to verify example directory contents before build

## [0.7.3] - 2026-11-15

### Fixed

- Docker build: fix `context` and `file` paths in `build-push-action` to use `./example` context with correct Dockerfile reference

## [0.7.2] - 2026-11-15

### Fixed

- Docker image build: replace removed `gcc:13-alpine` with `gcc:14-bookworm`, runtime with `debian:trixie-slim`, fix COPY paths for built artifacts, add `.dockerignore`

## [0.7.1] - 2026-11-15

### Fixed

- Docker image tag: replace invalid `.` in path (`ghcr.io/ai-warevo/.github/example` → `ghcr.io/ai-warevo/example`)

## [0.7.0] - 2026-11-15

### Added

- `build-example` job: cmake build of plugin + host, upload as release artifacts
- `docker` job: `docker/build-push-action` to build and push to GHCR (was broken docker-save)
- Release assets: `libplugin.so` and `host` binaries included in release

## [0.6.2] - 2026-11-15

### Fixed

- YAML parsing error in `changelog-release` action (nested quotes in `description`)

## [0.6.0] - 2026-11-15

### Added

- `make release` / `make release-dry-run` targets for automated SemVer bumps from CHANGELOG
- `scripts/release.sh` and `scripts/release.ps1` — parse [Unreleased] categories, bump version, commit + tag
- Release workflow deployed to `.github/workflows/` for this repo (not just `ci/` templates)
- Docker build + GHCR push job in release workflow

## [0.5.0] - 2026-11-15

### Added

- Release workflow deployed to `.github/workflows/` for this repo
- Docker build + GHCR push job in release workflow

### Changed

- Composite actions pinned to commit SHA instead of `@main`

## [0.4.0] - 2026-11-15

### Added

- Composite actions: `collect-release-assets`, `docker-save`, `docker-push-ghcr`, `download-artifacts`, `checksums-verify`
- Docker build + GHCR push job in release workflow
- Multi-stage Dockerfile for example (C++23, Alpine, gcc13)

### Changed

- Split `checksums` action into generate-only + `checksums-verify` for reusability
- `release.yml`: added `packages: write, actions: write` permissions, `concurrency` block
- `ci.yml`, `test.yml`: added `packages: write` + `concurrency`
- Removed stale todo directories

## [0.3.0] - 2026-11-15

### Added

- `example/` CMake project with C++23, Catch2 v3, per-test discovery, sanitizer and coverage support
- CI workflow for example: tests + coverage reports (gcovr, XML + HTML) + coverage badge deploy
- Sanitizer CI jobs: AddressSanitizer + UBSan, ThreadSanitizer (with ASLR relaxation)
- README badges: License, CI, Coverage (from tpl pattern)

### Changed

- `.gitignore`: added `build` to exclude CMake build directories

## [0.2.0] - 2026-11-15

### Added

- Composite action `release-version`: resolves version from tag, workflow_dispatch input, or run_number
- Composite action `checksums`: generates and verifies SHA256SUMS for release artifacts
- `workflow_dispatch` trigger with optional `version` input in `release.yml`
- `softprops/action-gh-release` with `generate_release_notes: false`
- `SHA256SUMS.txt` included in release assets

### Changed

- `release.yml` refactored: version resolution extracted to composite action
- `release.yml` uses `fetch-depth: 0` for full changelog history

## [0.1.0] - 2026-11-15

### Added

- Infrastructure template repository: skills, templates, CI/CD configs, editor settings
- LLM server (llama.cpp) with Docker Compose and GPU support
- AI skills: analyst, planner, executor, skill-creator, git-github-push
- Project templates: overview, task, README, CHANGELOG, CONTRIBUTING
- Universal CI/CD workflow templates (ci.yml, test.yml) for Node.js, Python, C#, C++, Go, Rust, Java
- Custom GitHub Action: SVG Badge Generator
- Coverage badge generation and deployment to `badges` branch
- Coverage HTML report deployment to `gh-pages`

[0.3.0]: https://github.com/ai-warevo/.github/compare/v0.2.0...v0.3.0
[0.2.0]: https://github.com/ai-warevo/.github/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/ai-warevo/.github/releases/tag/v0.1.0
