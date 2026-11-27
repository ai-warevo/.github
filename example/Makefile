VERSION ?= 0.0.1
PRESET  ?= gcc-15-debug

# ── Format & Lint ──
CLANG_FORMAT ?= clang-format-18
CLANG_TIDY   ?= clang-tidy-18
LINT_PRESET  ?= clang18-libcxx-debug

# ── Coverage ──
COVERAGE_PRESET ?= gcc-15-coverage

# ── Sanitizers ──
ASAN_PRESET ?= gcc-15-asan
TSAN_PRESET ?= gcc-15-tsan

# ── File discovery ──
CPP_EXTS      := cpp cxx c hpp hxx h
CPP_FIND_ARGS := $(patsubst %,-o -name '*.%',$(CPP_EXTS))
CPP_EXT_REGEX := \.($(subst $(eval) ,|,$(CPP_EXTS)))$$
SRC_EXTS      := cpp cxx c

# Deferred: not evaluated until the variable is used, so `make clean` doesn't run find/git
CPP_FILES        = $(shell find src include tests -type f \( -false $(CPP_FIND_ARGS) \) 2>/dev/null)
STAGED_CPP_FILES = $(shell git diff --cached --name-only --diff-filter=ACMR 2>/dev/null | grep -E '$(CPP_EXT_REGEX)' || true)

# Source files only (exclude headers) for clang-tidy to avoid false positives
CPP_SOURCE_FILES = $(shell find src tests -type f \( -false $(patsubst %,-o -name '*.%',$(SRC_EXTS)) \) 2>/dev/null)

# Debug/Release install config: check for explicit -debug suffix to avoid false matches
CMAKE_INSTALL_CONFIG := $(if $(filter %-debug,$(PRESET)),Debug,Release)

.PHONY: help format format-staged lint lint-staged build test asan tsan coverage clean

help:
	@echo "Reusable Makefile (v$(VERSION), default PRESET=$(PRESET))"
	@echo ""
	@echo "Targets:"
	@echo "  make help           Show this help message"
	@echo "  make format         Format all C/C++ files with clang-format"
	@echo "  make format-staged  Format staged C/C++ files and re-add to git"
	@echo "  make lint           Run clang-format check and clang-tidy ($(LINT_PRESET))"
	@echo "  make lint-staged    Run lint checks on staged C/C++ files ($(LINT_PRESET))"
	@echo "  make build          Configure, build, and install (uses PRESET)"
	@echo "  make test           Run ctest suite (uses PRESET)"
	@echo "  make asan           Build and run tests with ASan + UBSan ($(ASAN_PRESET))"
	@echo "  make tsan           Build and run tests with TSan ($(TSAN_PRESET))"
	@echo "  make coverage       Build, test, and generate coverage report ($(COVERAGE_PRESET))"
	@echo "  make clean          Remove build/ and dist/ directories"
	@echo ""
	@echo "Presets:"
	@echo "  gcc-15-debug | gcc-15-coverage | gcc-15-release | gcc-15-asan | gcc-15-tsan"
	@echo "  clang18-libcxx-debug | clang18-libcxx-release"
	@echo "  clang18-libstdcxx-debug | clang18-libstdcxx-release"
	@echo "  msvc-debug | msvc-release"
	@echo ""
	@echo "Examples:"
	@echo "  make build PRESET=gcc-15-debug"
	@echo "  make test  PRESET=gcc-15-debug"

format:
	@if [ -n "$(CPP_FILES)" ]; then \
		$(CLANG_FORMAT) -i $(CPP_FILES); \
	fi

format-staged:
	@if [ -n "$(STAGED_CPP_FILES)" ]; then \
		$(CLANG_FORMAT) -i $(STAGED_CPP_FILES); \
		git add $(STAGED_CPP_FILES); \
	else \
		echo "No staged C/C++ files to format"; \
	fi

lint:
	@if ! command -v $(CLANG_FORMAT) >/dev/null 2>&1 || ! command -v $(CLANG_TIDY) >/dev/null 2>&1; then \
		echo "SKIP: lint tools not found"; \
		exit 0; \
	fi; \
	if [ ! -f "build/$(LINT_PRESET)/compile_commands.json" ]; then \
		echo "Generating compile_commands.json for linting..."; \
		cmake --preset $(LINT_PRESET); \
	fi; \
	if [ -n "$(CPP_FILES)" ]; then \
		$(CLANG_FORMAT) --dry-run --Werror $(CPP_FILES); \
	fi; \
	if [ -n "$(CPP_SOURCE_FILES)" ]; then \
		$(CLANG_TIDY) -p build/$(LINT_PRESET) --warnings-as-errors='*' $(CPP_SOURCE_FILES); \
	else \
		echo "OK (no source files to lint)"; \
	fi

lint-staged:
	@STAGED=$$(git diff --cached --name-only --diff-filter=ACMR 2>/dev/null | grep -E '$(CPP_EXT_REGEX)' || true); \
	if [ -n "$$STAGED" ]; then \
		$(CLANG_FORMAT) --dry-run --Werror $$STAGED; \
		if [ ! -f "build/$(LINT_PRESET)/compile_commands.json" ]; then \
			echo "Generating compile_commands.json for linting..."; \
			cmake --preset $(LINT_PRESET); \
		fi; \
		STAGED_SRC=$$(echo "$$STAGED" | tr ' ' '\n' | grep -E '\.($(subst $(eval) ,|,$(SRC_EXTS)))$$' || true); \
		if [ -n "$$STAGED_SRC" ]; then \
			$(CLANG_TIDY) -p build/$(LINT_PRESET) --warnings-as-errors='*' $$STAGED_SRC; \
		fi; \
	else \
		echo "No staged C/C++ files to lint"; \
	fi

build:
	cmake --preset $(PRESET)
	cmake --build --preset $(PRESET) --parallel
	cmake --install build/$(PRESET) --config $(CMAKE_INSTALL_CONFIG) --prefix dist/$(PRESET)

test:
	ctest --preset $(PRESET) --output-on-failure

asan:
	$(MAKE) build PRESET=$(ASAN_PRESET)
	$(MAKE) test  PRESET=$(ASAN_PRESET)

tsan:
	$(MAKE) build PRESET=$(TSAN_PRESET)
	@if command -v setarch >/dev/null 2>&1; then \
		setarch $$(uname -m) -R $(MAKE) test PRESET=$(TSAN_PRESET); \
	else \
		$(MAKE) test PRESET=$(TSAN_PRESET); \
	fi

coverage:
	@if ! command -v gcovr >/dev/null 2>&1; then \
		echo "ERROR: gcovr not found"; \
		exit 1; \
	fi
	$(MAKE) build PRESET=$(COVERAGE_PRESET)
	$(MAKE) test  PRESET=$(COVERAGE_PRESET)
	@GCOV_CMD=$$(grep '^COVERAGE_GCOV_EXECUTABLE' build/$(COVERAGE_PRESET)/CMakeCache.txt 2>/dev/null | cut -d= -f2); \
	if [ -z "$$GCOV_CMD" ]; then \
		GCOV_CMD="gcov"; \
	fi; \
	if ! command -v $$GCOV_CMD >/dev/null 2>&1; then \
		echo "ERROR: $$GCOV_CMD not found"; \
		exit 1; \
	fi; \
	mkdir -p htmlcov; \
	PWD_ABS=$$(pwd); \
	FILTER_ARGS="--filter $$PWD_ABS/plugin/src/ --filter $$PWD_ABS/plugin/tests/ --filter $$PWD_ABS/host/src/ --filter $$PWD_ABS/include/"; \
	if [ -n "$(GCOVR_FILTER)" ]; then \
		FILTER_ARGS="$$FILTER_ARGS --filter $$PWD_ABS/$(GCOVR_FILTER)"; \
	fi; \
	EXCLUDE_ARGS="--exclude '.*/_deps/'"; \
	if [ -n "$(GCOVR_EXCLUDE)" ]; then \
		EXCLUDE_ARGS="$$EXCLUDE_ARGS --exclude $(GCOVR_EXCLUDE)"; \
	fi; \
	gcovr -r . build/$(COVERAGE_PRESET) --gcov-executable $$GCOV_CMD \
		$$FILTER_ARGS $$EXCLUDE_ARGS \
		--gcov-ignore-errors=no_working_dir_found \
		--exclude-unreachable-branches \
		--exclude-throw-branches \
		--print-summary \
		--xml coverage.xml \
		--html-details htmlcov/index.html; \
	echo "✓ Coverage report: htmlcov/index.html"

clean:
	rm -rf build dist htmlcov coverage.xml
