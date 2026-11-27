# Coverage Parser Action

A composite GitHub Action for extracting the coverage percentage from `coverage.xml` or `coverage.lcov`.

## Input parameters (`inputs`)

| Parameter | Description | Required | Default |
| :--- | :--- | :---: | :--- |
| `xml_path` | Path to `coverage.xml` | No | `coverage.xml` |
| `lcov_path` | Path to `coverage.lcov` | No | `coverage.lcov` |

## Output parameters (`outputs`)

| Parameter | Description | Example |
| :--- | :--- | :--- |
| `percent` | Coverage percentage | `87%` |
| `color` | Badge color (HEX) | `#44cc11` |

## Color logic

| Range | Color |
| :--- | :--- |
| `< 50%` | `#e05d44` (red) |
| `50–79%` | `#dfb317` (yellow) |
| `≥ 80%` | `#44cc11` (green) |

## Usage example

```yaml
- name: Parse Coverage
  id: coverage
  uses: ai-warevo/<repo>/.github/actions/coverage-parser@72e62471355bf347d09c7e183af5fd33bf41fbaf

- name: Generate Badge
  uses: ai-warevo/<repo>/.github/actions/badge@72e62471355bf347d09c7e183af5fd33bf41fbaf
  with:
    text_right: '${{ steps.coverage.outputs.percent }}'
    bg_color_right: '${{ steps.coverage.outputs.color }}'

- name: Create Release
  uses: softprops/action-gh-release@v2
  with:
    body: 'Coverage: ${{ steps.coverage.outputs.percent }}'
```
