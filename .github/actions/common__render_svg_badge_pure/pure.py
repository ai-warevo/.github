"""Pure function: generate SVG badge with dynamic width calculation."""

import os
import json
import sys
from pathlib import Path


def render_svg_badge(
    width: int,
    height: int,
    text_left: str,
    text_right: str,
    bg_color_left: str,
    bg_color_right: str,
    text_color: str,
) -> str:
    """Render SVG badge with two-segment layout.
    
    Dynamic calculation:
      HALF_WIDTH = width / 2
      QUARTER_WIDTH = width / 4
      THREE_QUARTERS = 3 * width / 4
    """
    half_width = width // 2
    quarter_width = width // 4
    three_quarters = (width * 3) // 4
    
    # Text baseline for height 20 is y="15" (SVG coordinate system)
    # Adjust based on actual height: y = height - 5
    text_y = height - 5
    
    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}">
    <linearGradient id="b" x2="0" y2="100%">
        <stop offset="0" stop-color="#bbb" stop-opacity=".1"/>
        <stop offset="1" stop-opacity=".1"/>
    </linearGradient>
    <mask id="a">
        <rect width="{width}" height="{height}" rx="3" fill="#fff"/>
    </mask>
    <g mask="url(#a)">
        <rect width="{half_width}" height="{height}" fill="{bg_color_left}"/>
        <rect x="{half_width}" width="{half_width}" height="{height}" fill="{bg_color_right}"/>
        <rect width="{width}" height="{height}" fill="url(#b)"/>
    </g>
    <g fill="{text_color}" text-anchor="middle" font-family="DejaVu Sans,Verdana,Geneva,sans-serif" font-size="11">
        <text x="{quarter_width}" y="{text_y + 1}" fill="#010101" fill-opacity=".3">{text_left}</text>
        <text x="{quarter_width}" y="{text_y}">{text_left}</text>
        <text x="{three_quarters}" y="{text_y + 1}" fill="#010101" fill-opacity=".3">{text_right}</text>
        <text x="{three_quarters}" y="{text_y}">{text_right}</text>
    </g>
</svg>'''
    
    return svg


def main():
    width = int(os.environ.get('INPUT_WIDTH', '120'))
    height = int(os.environ.get('INPUT_HEIGHT', '20'))
    text_left = os.environ.get('INPUT_TEXT_LEFT', 'coverage')
    text_right = os.environ.get('INPUT_TEXT_RIGHT', '80%')
    bg_color_left = os.environ.get('INPUT_BG_COLOR_LEFT', '#555')
    bg_color_right = os.environ.get('INPUT_BG_COLOR_RIGHT', '#44cc11')
    text_color = os.environ.get('INPUT_TEXT_COLOR', '#fff')
    filename = os.environ.get('INPUT_FILENAME', 'badge.svg')
    
    # Render SVG
    svg_content = render_svg_badge(
        width=width,
        height=height,
        text_left=text_left,
        text_right=text_right,
        bg_color_left=bg_color_left,
        bg_color_right=bg_color_right,
        text_color=text_color,
    )
    
    # Write to file (use relative path, not absolute)
    badge_path = Path(filename)
    badge_path.parent.mkdir(parents=True, exist_ok=True)
    with open(badge_path, 'w', encoding='utf-8') as f:
        f.write(svg_content)
    
    output = {
        'badge_path': str(badge_path),
        'svg': svg_content,
    }
    
    # Write to GITHUB_OUTPUT
    gh_out = os.environ.get('GITHUB_OUTPUT')
    if gh_out:
        with open(gh_out, 'a', encoding='utf-8') as f:
            f.write(f"json_output={json.dumps(output, ensure_ascii=False)}\n")
    else:
        print("ERROR: GITHUB_OUTPUT not set", file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
