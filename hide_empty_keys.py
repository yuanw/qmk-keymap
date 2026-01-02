#!/usr/bin/env python3
"""
Script to add visibility="hidden" to SVG rect elements for keys
that don't have any tap text content in the keymap.
"""

import re
import sys
from pathlib import Path


def has_tap_text(key_group_content: str) -> bool:
    """Check if a key group has tap text (not just hold/shifted text)."""
    # Look for <text> elements with class containing "tap"
    # Pattern: <text ... class="... tap ...">content</text>
    tap_pattern = r'<text[^>]*class="[^"]*\btap\b[^"]*"[^>]*>([^<]+|<[^/].*?)</text>'
    matches = re.findall(tap_pattern, key_group_content, re.DOTALL)

    for match in matches:
        # Check if there's actual text content (not just whitespace or empty)
        # Also handle tspan elements
        text_content = re.sub(r'<[^>]+>', '', match).strip()
        if text_content and text_content != '▽':  # ▽ is trans key symbol
            return True

    # Also check for tspan content inside tap text
    tspan_pattern = r'<text[^>]*class="[^"]*\btap\b[^"]*"[^>]*>.*?<tspan[^>]*>([^<]+)</tspan>'
    tspan_matches = re.findall(tspan_pattern, key_group_content, re.DOTALL)
    for match in tspan_matches:
        if match.strip():
            return True

    return False


def is_trans_key(key_group_content: str) -> bool:
    """Check if a key is a transparent key (shows ▽)."""
    return 'class="key trans' in key_group_content or 'class="key trans tap"' in key_group_content


def process_svg(svg_content: str, hide_trans: bool = False) -> str:
    """
    Process SVG content and add visibility="hidden" to rect elements
    in key groups that don't have tap text content.

    Args:
        svg_content: The SVG file content
        hide_trans: If True, also hide transparent keys (▽)

    Returns:
        Modified SVG content
    """
    # Pattern to match key groups: <g ... class="key keypos-N">...</g>
    key_group_pattern = r'(<g[^>]*class="[^"]*key[^"]*keypos-\d+[^"]*"[^>]*>)(.*?)(</g>)'

    def replace_key_group(match):
        g_open = match.group(1)
        g_content = match.group(2)
        g_close = match.group(3)

        # Check if this key has tap text
        has_text = has_tap_text(g_content)
        is_trans = is_trans_key(g_open + g_content)

        # Determine if we should hide this key
        should_hide = not has_text or (hide_trans and is_trans)

        if should_hide:
            # Add visibility="hidden" to the rect element if not already present
            if 'visibility="hidden"' not in g_content:
                # Replace <rect with <rect visibility="hidden"
                g_content = re.sub(
                    r'<rect\s+',
                    '<rect visibility="hidden" ',
                    g_content,
                    count=1
                )
        else:
            # Remove visibility="hidden" if present (for keys that should be visible)
            g_content = re.sub(
                r'visibility="hidden"\s*',
                '',
                g_content
            )

        return g_open + g_content + g_close

    return re.sub(key_group_pattern, replace_key_group, svg_content, flags=re.DOTALL)


def main():
    import argparse

    parser = argparse.ArgumentParser(
        description='Add visibility="hidden" to empty keys in SVG keymap'
    )
    parser.add_argument(
        'svg_file',
        type=Path,
        help='Path to the SVG file to process'
    )
    parser.add_argument(
        '-o', '--output',
        type=Path,
        help='Output file path (default: overwrite input file)'
    )
    parser.add_argument(
        '--hide-trans',
        action='store_true',
        help='Also hide transparent keys (▽)'
    )
    parser.add_argument(
        '--dry-run',
        action='store_true',
        help='Print changes without modifying the file'
    )

    args = parser.parse_args()

    if not args.svg_file.exists():
        print(f"Error: File not found: {args.svg_file}", file=sys.stderr)
        sys.exit(1)

    # Read input
    svg_content = args.svg_file.read_text(encoding='utf-8')

    # Process
    modified_content = process_svg(svg_content, hide_trans=args.hide_trans)

    if args.dry_run:
        # Show diff
        import difflib
        diff = difflib.unified_diff(
            svg_content.splitlines(keepends=True),
            modified_content.splitlines(keepends=True),
            fromfile=str(args.svg_file),
            tofile=str(args.svg_file) + ' (modified)'
        )
        print(''.join(diff))
    else:
        # Write output
        output_path = args.output or args.svg_file
        output_path.write_text(modified_content, encoding='utf-8')
        print(f"Processed: {args.svg_file} -> {output_path}")


if __name__ == '__main__':
    main()
