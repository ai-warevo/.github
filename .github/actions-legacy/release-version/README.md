# Release Version Resolver

A composite GitHub Action for resolving the release version.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `version` | Manual version (e.g., `1.2.3`) | No | `''` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `release_version` | Version with `v` prefix | `v1.2.3` |
| `release_version_stripped` | Version without `v` prefix | `1.2.3` |

## Resolution logic

1. **Tag** (`push: tags: v*`) → `github.ref_name`
2. **Workflow dispatch** with `version` input → input without `v`
3. **Fallback** → `v0.0.<run_number>`

## Usage example

```yaml
steps:
  - name: Resolve version
    id: version
    uses: ai-warevo/<repo>/.github/actions/release-version@72e62471355bf347d09c7e183af5fd33bf41fbaf
    with:
      version: ${{ github.event.inputs.version }}
```
