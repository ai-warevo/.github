# Release Commit and Tag

Commits the promoted `CHANGELOG.md` and `VERSION`, creates an annotated release tag, and pushes the commit and tag to `origin`.

## Inputs

| Input | Required | Description |
| --- | --- | --- |
| `version` | Yes | Release version with `v` prefix, for example `v1.2.3` |
| `branch` | Yes | Branch that receives the release commit |

The action fails if the tag already exists or if there are no staged release metadata changes. The calling workflow must grant `contents: write` and check out the repository with credentials that can push.

## Usage

```yaml
- name: Commit and tag release
  uses: ai-warevo/.github/.github/actions/release-commit-tag@<40-char-SHA>
  with:
    version: ${{ needs.version-derive.outputs.release_version }}
    branch: ${{ github.ref_name }}
```
