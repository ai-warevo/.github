# Docker Build and Push Action

A composite GitHub Action for building and pushing a Docker image to a container registry.

## Purpose

Wraps the standard Docker pipeline: `setup-buildx` → `login` → `build-push`. Supports both push and local load modes.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `context` | Docker build context | No | `.` |
| `dockerfile` | Dockerfile path (relative to repository root, NOT context) | No | `Dockerfile` |
| `registry` | Container registry URL | No | `ghcr.io` |
| `image_name` | Image name (e.g. `org/repo`) | Yes | — |
| `tags` | Docker image tags (one per line) | Yes | — |
| `push` | Push the image | No | `true` |
| `load` | Load the image into the local Docker daemon | No | `false` |
| `github_token` | GitHub token for registry authentication | No | `${{ github.token }}` |

## Usage example

```yaml
- name: Build and push Docker image
  uses: ai-warevo/<repo>/.github/actions/docker-build-push@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    context: ./example
    dockerfile: example/Dockerfile
    registry: ghcr.io
    image_name: ai-warevo/example
    tags: |
      ghcr.io/ai-warevo/example:1.2.3
      ghcr.io/ai-warevo/example:latest
```
