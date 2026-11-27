# Docker Save Image Action

A composite GitHub Action for saving a Docker image as a tar file and uploading it as a GitHub Actions artifact.

## Purpose

Exports a Docker image to a tar file and saves it as a GitHub Actions artifact. Used to transfer images between jobs (e.g. build → release) as a companion to [docker-push-ghcr](.github/actions/docker-push-ghcr/README.md).

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `image` | Docker image name (e.g. `myapp/backend`) | Yes | — |
| `tag` | Image tag (e.g. `1.2.3` or `latest`) | No | `latest` |
| `output` | Output tar filename | No | `image.tar` |
| `artifact_name` | GitHub Actions artifact name | No | `docker-image` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `image` | Full image name with tag | `myapp/backend:1.2.3` |
| `artifact_name` | Name of the uploaded artifact | `docker-image` |

## Usage example

```yaml
- name: Save Docker image
  uses: ai-warevo/<repo>/.github/actions/docker-save@72e62471355bf347d09c7e183af5fd33bf41fbaf
  with:
    image: myapp/backend
    tag: '1.2.3'
    output: backend-image.tar
```
