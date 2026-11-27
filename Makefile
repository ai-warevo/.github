# Language-agnostic Makefile contract for ai-warevo projects.
# Consumer repos define these targets at their root.
#
# Release process (no local scripts needed):
#   1. Add categorized entries under ## [Unreleased] in CHANGELOG.md
#   2. Trigger the release workflow:
#      make release          # GitHub Actions: w orkflow_dispatch, derives version
#      make release VERSION=1.2.3  # GitHub Actions: workflow_dispatch with override

.PHONY: pr-fast weekly-deep release release-dry-run

# ═══════════════════════════════════════════════════════════
#  SYSTEM — do not modify, this is infrastructure
# ═══════════════════════════════════════════════════════════

# Release via GitHub Actions workflow_dispatch (no local scripts).
release:
	gh workflow run release.yml --ref main $(if $(VERSION),--field version=$(VERSION))

release-dry-run:
	@echo "Preview release: add categorized entries under ## [Unreleased] in CHANGELOG.md"
	@echo "Then run: make release"
	@echo ""
	@echo "Version derivation from categories:"
	@echo "  Breaking, Removed → major"
	@echo "  Added, Deprecated → minor"
	@echo "  Fixed, Changed, Security → patch"

pr-fast:
	gh workflow run pr-fast.yml --ref main

weekly-deep:
	gh workflow run weekly-deep.yml --ref main

# ═══════════════════════════════════════════════════════════
#  YOURS — add your custom targets here
# ═══════════════════════════════════════════════════════════

# Example:
# dev:
# 	cmake -B build/dev -G Ninja -DCMAKE_BUILD_TYPE=Debug
# 	cmake --build build/dev --parallel
#
# lint:
# 	@echo "run your linter"
#
# test:
# 	@echo "run your tests"
