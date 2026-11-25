# Language-Agnostic CI/CD

This repository provides reusable GitHub Actions workflows for projects in `ai-warevo`. The shared workflows orchestrate a small Makefile contract; each project owns its build, test, coverage, and release commands. This keeps the CI control plane language-agnostic across C++, C#, Python, JavaScript/TypeScript, Go, Rust, and other stacks.

## Three workflows

| Workflow | Trigger | Purpose |
| --- | --- | --- |
| `pr-fast` | Pull request to main/master | Lint, build, unit tests — fast feedback |
| `weekly-deep` | Weekly cron (Mon 00:00 UTC), PR, manual | E2E, integration, sanitizers (ASan, TSan, UBSan) |
| `release` | Tag push (`v*`) or manual | Build, release, publish Docker image. Auto-derives version from changelog categories. |

A failed check prevents downstream release publication. No shared workflow assumes a particular compiler, package manager, or test framework.

## Makefile contract

Each consumer repository must define these targets at its root:

- `pr-fast` — quick lint/format checks, build, and unit tests. For C++, a common choice is ASan + UBSan.
- `weekly-deep` — expensive stress, end-to-end/integration, security, or race-detection checks. For C++, this may include TSan and strict UBSan.
- `release` — trigger the release workflow (changelog → version derivation → tag → release).

Targets should fail with a nonzero status if a required command fails. The reusable workflows invoke only these names.

Example sanitizer flags for C++:

```make
# Fast memory/undefined-behavior checks
-fsanitize=address,undefined -fno-omit-frame-pointer

# Separate deep race-detection build (do not combine with ASan)
-fsanitize=thread -fno-omit-frame-pointer

# Strict UBSan variant
-fsanitize=undefined -fno-sanitize-recover=all
```

## Three-tier architecture

| Tier | Location | Purpose |
| --- | --- | --- |
| **Reusable workflows** | `.github/workflows/reusable-*.yml` | Full CI logic, `workflow_call` trigger, parameterized inputs |
| **Templates** | `templates/workflows/*.yml` | Thin wrappers — consumers copy and customize `name`/`on:`/`with:` |
| **Monorepo workflows** | `.github/workflows/*.yml` | Internal CI for this monorepo and example project |

Reusable workflows are the **source of truth** — updates propagate automatically to all consumers.
Templates are thin wrappers: consumer copies, changes only `name`, `on:`, and `with:`.

### Reusable workflows

| Workflow | Purpose | Trigger |
| --- | --- | --- |
| `reusable-pr-fast.yml` | Lint, build, unit tests | `workflow_call`; optional `setup_command` prepares generated/source files after checkout in each job |
| `reusable-weekly-deep.yml` | E2E, integration, sanitizers | `workflow_call` |
| `reusable-release.yml` | Build, release, publish Docker | `workflow_call` |

### Templates (for consumers)

Consumers copy from `templates/workflows/` into their `.github/workflows/` and customize.

| Template | Trigger | Purpose |
| --- | --- | --- |
| `pr-fast.yml` | `pull_request` to main/master | Lint, build, unit tests |
| `weekly-deep.yml` | `schedule` (weekly), `workflow_dispatch` | Sanitizers + coverage |
| `release.yml` | `push` (v* tags), `workflow_dispatch` | Build, release, Docker. Auto-derives version from changelog categories unless overridden. |

### Monorepo workflows

| Workflow | Purpose |
| --- | --- |
| `pr-fast.yml` | Monorepo PR checks (lint + build + test) |
| `release.yml` | Monorepo release pipeline |
| `weekly-deep.yml` | Monorepo weekly deep checks |
| `example-ci.yml` | Example project CI (tests + coverage + sanitizers) |
| `actions-test.yml` | Action test dispatcher (matrix of 21 actions) |

The commit validator permits the standard types `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, and `revert`, with optional scope and breaking marker; maximum subject length is 88 characters. The generated `release vX.Y.Z` commit is exempt.

## Setup

1. Copy the desired template(s) from `templates/workflows/` into your project at `.github/workflows/`.
2. Customize `name`, `on:`, and `with:` inputs in the copied template. The `uses:` line should point to `ai-warevo/.github/.github/workflows/reusable-<name>.yml@<sha>`.
3. For stricter supply-chain security, replace `@main` in the `uses:` line with a pinned commit SHA.
4. Add the four Makefile targets (`pr-fast`, `weekly-deep`, `release`, `release-dry-run`) to the project Makefile.
5. Verify that the changelog follows Keep a Changelog, and that the release commit from `make release` pushes the annotated `vX.Y.Z` tag. The tag must have an exact `## [X.Y.Z]` or `## [X.Y.Z] - YYYY-MM-DD` section in `CHANGELOG.md`; missing or empty notes fail the gate.
6. Run all Makefile targets locally. Open a PR and verify the PR status check before relying on it as a branch protection rule.
7. Add the project's status and coverage badges to its README.

## Permissions and security

Each reusable workflow declares its own permissions. `reusable-pr-fast.yml` uses `contents: read`; `reusable-release.yml` uses `contents: write`, `packages: write`, `actions: write`; `reusable-weekly-deep.yml` uses `contents: write`, `packages: write`. Templates inherit these via `uses:`. Pin reusable workflow references to a reviewed commit SHA in production consumers. The copyable templates use `@main` for easy adoption — replace with a SHA for stricter supply-chain requirements.

## Composite Actions

All 23 composite actions live in `.github/actions/` and are referenced by consumer workflows using `ai-warevo/.github/.github/actions/<name>@<40-char-SHA>`. Each action has its own `README.md` with inputs, outputs, and usage examples.

| Action | Purpose |
| --- | --- |
| `badge/` | Generate custom SVG badges |
| `badge-deploy/` | Deploy SVG badge to target branch |
| `changelog-generate/` | Generate [Unreleased] section from git log (conventional commits) |
| `changelog-promote/` | Promote [Unreleased] to a version heading and write VERSION file |
| `changelog-release/` | Extract release notes from CHANGELOG.md by tag |
| `checksums/` | Generate SHA256SUMS.txt for files in a directory |
| `checksums-verify/` | Verify SHA256 checksums against a checksums file |
| `clang-format/` | Check C++ formatting with clang-format |
| `clang-tidy/` | Run clang-tidy on C++ source and headers |
| `cmake-build/` | Configure, build, and optionally install with CMake |
| `collect-release-assets/` | Collect build artifacts from multiple jobs for release |
| `coverage-html-deploy/` | Deploy coverage HTML report to GitHub Pages |
| `coverage-parser/` | Extract coverage percentage from coverage.xml or LCOV |
| `ctest-run/` | Run CTest for a CMake project |
| `docker-build-push/` | Build and push a Docker image to a container registry |
| `docker-push-ghcr/` | Push Docker image from artifact to GHCR |
| `docker-save/` | Save Docker image as tar and upload as artifact |
| `download-artifacts/` | Download and merge multiple artifacts by pattern |
| `gcovr-report/` | Generate coverage reports (XML + HTML) from CMake build |
| `release-version/` | Resolve release version from tag, input, or run_number |
| `release-commit-tag/` | Commit release metadata, create an annotated tag, and push both |
| `setup-toolchain/` | Compute CMake arguments for a given toolchain configuration |
