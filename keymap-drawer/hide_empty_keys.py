#!/usr/bin/env python3
"""
Script to add visibility="hidden" to SVG rect elements for specific key positions.
"""

import sys
from pathlib import Path

from bs4 import BeautifulSoup

# Key positions to hide (edges and unused positions)
HIDDEN_KEYPOS = {0, 11, 24, 35, 36, 37, 38, 39, 40, 41}


def process_svg(svg_content: str, hidden_positions: set[int] = HIDDEN_KEYPOS) -> str:
    """
    Process SVG content and add visibility="hidden" to rect elements
    in key groups with specified keypos classes.

    Args:
        svg_content: The SVG file content
        hidden_positions: Set of keypos numbers to hide

    Returns:
        Modified SVG content
    """
    soup = BeautifulSoup(svg_content, "xml")

    for pos in hidden_positions:
        # Find all g elements with class containing keypos-N
        for g in soup.find_all("g", class_=lambda c: c and f"keypos-{pos}" in c):
            rect = g.find("rect")
            if rect and not rect.get("visibility"):
                rect["visibility"] = "hidden"

    return str(soup.svg)


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description='Add visibility="hidden" to empty keys in SVG keymap'
    )
    parser.add_argument("svg_file", type=Path, help="Path to the SVG file to process")
    parser.add_argument(
        "-o", "--output", type=Path, help="Output file path (default: overwrite input)"
    )
    parser.add_argument(
        "--positions",
        type=str,
        help="Comma-separated list of keypos numbers to hide (default: 0,11,24,35,36-41)",
    )

    args = parser.parse_args()

    if not args.svg_file.exists():
        print(f"Error: File not found: {args.svg_file}", file=sys.stderr)
        sys.exit(1)

    # Parse positions if provided
    hidden_positions = HIDDEN_KEYPOS
    if args.positions:
        hidden_positions = set()
        for part in args.positions.split(","):
            if "-" in part:
                start, end = map(int, part.split("-"))
                hidden_positions.update(range(start, end + 1))
            else:
                hidden_positions.add(int(part))

    # Read input
    svg_content = args.svg_file.read_text(encoding="utf-8")

    # Process
    modified_content = process_svg(svg_content, hidden_positions)

    # Write output
    output_path = args.output or args.svg_file
    output_path.write_text(modified_content, encoding="utf-8")


if __name__ == "__main__":
    main()
