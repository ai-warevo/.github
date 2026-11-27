# Docker Push to GHCR Action

A composite GitHub Action for loading a Docker image from an artifact and pushing it to GitHub Container Registry.

## Purpose

Loads a Docker image from a tar artifact, tags it with `:vX.Y.Z` and optionally `:latest`, then pushes to GHCR. Used during the release phase as a companion to [docker-save](.github/actions/docker-save/README.md).

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `image` | Full GHCR image name (e.g. `ghcr.io/org/repo`) | Yes | — |
| `version` | Release version for tagging (e.g. `1.2.3`) | Yes | — |
| `artifact_name` | Name of the Docker image artifact | No | `docker-image` |
| `artifact_path` | Filename of the tar inside the artifact | No | `image.tar` |
| `token` | GitHub token for GHCR authentication | No | `${{ github.token }}` |
| `push_latest` | Also push `:latest` tag | No | `true` |

## Usage example

```yaml
- name: Push to GHCR
  uses: ai-warevo/<repo>/.github/actions/docker-push-ghcr@72e62471355bf347d09c7e183af5fd33bf41fbaf
  with:
    image: ghcr.io/myorg/myapp
    version: '1.2.3'
    token: ${{ secrets.GITHUB_TOKEN }}
```
