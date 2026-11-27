# Collect Release Assets Action

A composite GitHub Action for collecting and organizing build artifacts from multiple jobs for release.

## Purpose

Combines artifacts from multiple jobs into a single versioned directory and optionally generates SHA256 checksums. Follows the production pattern: parallel builds → collect → checksum → release.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `version` | Release version (e.g. `1.2.3`) | Yes | — |
| `output_dir` | Output directory for collected artifacts | No | `release-artifacts` |
| `include_checksums` | Generate SHA256 checksums | No | `true` |
| `github_token` | GitHub token for artifact download | No | `${{ github.token }}` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `artifact_path` | Path to the collected artifacts directory | `release-artifacts` |
| `checksum_file` | Path to the generated checksums file | `release-artifacts/SHA256SUMS.txt` |
| `asset_count` | Number of collected assets | `5` |

## Usage example

```yaml
- name: Collect release assets
  id: collect
  uses: ai-warevo/<repo>/.github/actions/collect-release-assets@72e62471355bf347d09c7e183af5fd33bf41fbaf
  with:
    version: '1.2.3'
    output_dir: release-artifacts
```
