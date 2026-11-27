# SVG Badge Generator Action

A custom action for quickly generating independent SVG badges without using third-party APIs or heavy dependencies.

## Input parameters (`inputs`)

You can customize the badge appearance through the `with` block:


| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `width` | Total SVG width in pixels | No | `120` |
| `height` | SVG height in pixels | No | `20` |
| `text_left` | Text on the left (gray) half | No | `coverage` |
| `text_right` | Text on the right (colored) half | No | `80%` |
| `bg_color_left` | Background color of the left part (HEX/name) | No | `#555` |
| `bg_color_right`| Background color of the right part (HEX/name)| No | `#44cc11` |
| `text_color` | Text color | No | `#fff` |
| `filename` | Path and filename to save the file | No | `badge.svg` |

## Output parameters (`outputs`)


| Parameter | Description | Example Value |
| :--- | :--- | :--- |
| `badge_path` | Path to the created SVG file | `assets/coverage.svg` |

## Complete usage example in a project

Example workflow that generates a badge and immediately saves it to your project repository:

```yaml
name: Update Project Status Badge

on:
  push:
    branches: [ main ]
  workflow_dispatch:

jobs:
  badge:
    runs-on: ubuntu-latest
    steps:
      - name: Checkout code
        uses: actions/checkout@v4

      - name: Generate Custom Badge
        id: generate_step
        uses: ai-warevo/<repo>/.github/actions/badge@72e62471355bf347d09c7e183af5fd33bf41fbaf
        with:
          width: '150'
          text_left: 'production'
          text_right: 'up'
          bg_color_right: '#007fff'
          filename: 'status-badge.svg'

      - name: Commit and Push Badge
        run: |
          git config --global user.name "github-actions[bot]"
          git config --global user.email "github-actions[bot]@users.noreply.github.com"
          git add status-badge.svg
          if git diff --staged --quiet; then
            echo "Badge did not change."
          else
            git commit -m "chore: update generated badge [skip ci]"
            git push
          fi
```

After the workflow runs, you can insert the badge into your `README.md` like this:
```markdown
![Status Badge](status-badge.svg)
```
