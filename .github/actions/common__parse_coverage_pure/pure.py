"""Pure function: parse coverage percentage and determine badge color."""

import os
import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def parse_cobertura_xml(xml_path: str) -> int | None:
    """Parse coverage percentage from Cobertura XML line-rate attribute."""
    path = Path(xml_path)
    if not path.exists():
        return None
    
    try:
        tree = ET.parse(xml_path)
        root = tree.getroot()
        
        # Get line-rate from root <coverage> element
        line_rate_str = root.get('line-rate', '0')
        try:
            line_rate = float(line_rate_str)
            percent = int(round(line_rate * 100))
            return max(0, min(100, percent))  # Clamp to 0-100
        except (ValueError, TypeError):
            return None
    except Exception:
        return None


def parse_lcov(lcov_path: str) -> int | None:
    """Parse coverage percentage from LCOV file (DA: lines)."""
    path = Path(lcov_path)
    if not path.exists():
        return None
    
    try:
        lines_found = 0
        lines_hit = 0
        
        with open(lcov_path, 'r', encoding='utf-8') as f:
            for line in f:
                line = line.strip()
                if line.startswith('LH:'):
                    # Lines Hit
                    lines_hit = int(line[3:])
                elif line.startswith('LF:'):
                    # Lines Found
                    lines_found = int(line[3:])
        
        if lines_found > 0:
            percent = int(round((lines_hit / lines_found) * 100))
            return max(0, min(100, percent))
        return None
    except Exception:
        return None


def get_color_for_percent(percent: int) -> tuple[str, str]:
    """Determine hex color and GitHub color based on coverage percentage.
    
    Returns:
        (hex_color, github_color)
    """
    if percent >= 80:
        return '#44cc11', 'brightgreen'
    elif percent >= 50:
        return '#dfb317', 'yellow'
    else:
        return '#e05d44', 'red'


def main():
    xml_path = os.environ.get('INPUT_XML_PATH', 'coverage.xml')
    lcov_path = os.environ.get('INPUT_LCOV_PATH', 'coverage.lcov')
    
    # Try XML first, then LCOV
    percent = parse_cobertura_xml(xml_path)
    if percent is None:
        percent = parse_lcov(lcov_path)
    
    # Default to 0 if both fail
    if percent is None:
        percent = 0
    
    hex_color, github_color = get_color_for_percent(percent)
    
    output = {
        'percent': percent,
        'color': hex_color,
        'github_color': github_color,
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
