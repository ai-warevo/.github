# Badge Deploy Action

A composite GitHub Action for deploying an SVG badge file to a target branch (self-contained, no external actions required).

## Purpose

Copies an SVG badge to a staging directory and deploys it by pushing directly to the target branch. Useful for maintaining coverage badges, status badges, or other SVG images on a dedicated branch like `badges`.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `badge_path` | Path to the SVG badge file (relative to workspace) | Yes | — |
| `badge_filename` | Filename to use in the deploy folder | No | `basename of badge_path` |
| `branch` | Target branch to deploy to | No | `badges` |
| `target_folder` | Target folder on the branch | No | `.` |
| `token` | GitHub token for authentication | No | Runner token |

## Usage example

```yaml
- name: Deploy Coverage Badge
  uses: ai-warevo/<repo>/.github/actions/badge-deploy@6cae36f7fe728c23b1a6170d09d6dace55edea77
  with:
    badge_path: coverage.svg
    branch: badges
    target_folder: .
```
