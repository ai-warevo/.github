# Changelog Release Action

A composite GitHub Action for extracting release notes from `CHANGELOG.md` and generating a release body for GitHub Release.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `tag` | Release tag (e.g., `v1.2.3`) | No | `${{ github.ref_name }}` |
| `changelog` | Path to `CHANGELOG.md` | No | `CHANGELOG.md` |
| `release-body` | Filename for release body | No | `release_body.md` |
| `what-included` | "What's Included" section | No | — |
| `quick-start` | "Quick Start" section | No | — |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `body_path` | Path to the release body file | `release_body.md` |
| `version` | Version without `v` prefix | `1.2.3` |
| `notes` | Extracted release notes | (multiline) |

## Usage example

```yaml
name: Release

on:
  push:
    tags:
      - 'v*'

permissions:
  contents: write

jobs:
  release:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout
        uses: actions/checkout@v4

      - name: Generate Release Body
        id: changelog
        uses: ai-warevo/<repo>/.github/actions/changelog-release@72e62471355bf347d09c7e183af5fd33bf41fbaf
        with:
          tag: ${{ github.ref_name }}
          changelog: 'CHANGELOG.md'
          release-body: 'release_body.md'

      - name: Create Release
        uses: softprops/action-gh-release@v2
        with:
          body_path: ${{ steps.changelog.outputs.body_path }}
```

## CHANGELOG.md format

The action parses the [Keep a Changelog](https://keepachangelog.com/) format:

```markdown
## [1.2.3] - 2024-01-15
### Added
- New feature X
### Fixed
- Bug Y

[1.2.3]: https://github.com/...
```

Notes are extracted between `## [version]` and the next `## [` heading or link reference block.
