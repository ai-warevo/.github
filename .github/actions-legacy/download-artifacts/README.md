# Download Artifacts Action

A composite GitHub Action for downloading multiple artifacts by pattern and merging them into a single directory.

## Purpose

A wrapper around `actions/download-artifact@v4` for conveniently downloading multiple artifacts by glob pattern and merging them into a single directory. Useful for collecting build outputs from parallel jobs.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `pattern` | Glob pattern to match artifact names | No | `*` |
| `merge-multiple` | Merge artifacts into a single directory | No | `true` |
| `path` | Directory to download artifacts into | No | `downloaded-artifacts` |
| `github_token` | GitHub token for authentication | No | `${{ github.token }}` |

## Usage example

```yaml
- name: Download all release artifacts
  uses: ai-warevo/<repo>/.github/actions/download-artifacts@72e62471355bf347d09c7e183af5fd33bf41fbaf
  with:
    pattern: '*'
    merge-multiple: true
    path: artifacts/
```
