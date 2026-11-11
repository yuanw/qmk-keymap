#!/usr/bin/env python3
"""Generate a self-contained keymap trainer page from keymap-drawer output.

Reads the processed keymap-drawer YAML (pretty tap/hold/shift labels) plus the
QMK geometry JSON used for the SVG, and emits one HTML file with no deps:
open it in a browser and type on the real keyboard.

    generate_trainer.py <target> --yaml out.yaml --geom info.json \
        --layout LAYOUT --out trainer.html
Combos (b = ; + ., w = m + g) and the tap of layer-tap thumbs (r = tap R_NUM)
are read from yuanw.c, since the drawn labels do not carry them.
"""

import argparse
import json
import re
from pathlib import Path

import yaml

# Multi-char labels that a browser keydown can actually produce. Anything not
# listed here (macros, DPI, mouse, layer keys) is shown on the board but left
# out of the drills, since the browser cannot tell it apart from a shortcut.
TYPABLE = {
    "Spc": " ",
    "Esc": "Escape",
    "BSPC": "Backspace",
    "Tab": "Tab",
    "Enter": "Enter",
    "DEL": "Delete",
    "Ins": "Insert",
    "PGUP": "PageUp",
    "PGDN": "PageDown",
    "HOME": "Home",
    "END": "End",
    "LEFT": "ArrowLeft",
    "RIGHT": "ArrowRight",
    "Up": "ArrowUp",
    "Down": "ArrowDown",
    "△": "ArrowUp",
    "▽": "ArrowDown",
    "◁": "ArrowLeft",
    "▷": "ArrowRight",
}
for _n in range(1, 13):
    TYPABLE[f"F{_n}"] = f"F{_n}"

# KC_ names -> what the browser reports in event.key.
KC_CHARS = {
    "KC_MINUS": "-", "KC_EQUAL": "=", "KC_LEFT_BRACKET": "[", "KC_RIGHT_BRACKET": "]",
    "KC_BACKSLASH": "\\", "KC_SEMICOLON": ";", "KC_QUOTE": "'", "KC_COMMA": ",",
    "KC_DOT": ".", "KC_SLASH": "/", "KC_GRAVE": "`", "KC_SPACE": " ",
    "KC_TAB": "Tab", "KC_ENTER": "Enter", "KC_ESCAPE": "Escape", "KC_BACKSPACE": "Backspace",
    "KC_DELETE": "Delete", "KC_INSERT": "Insert", "KC_PAGE_UP": "PageUp",
    "KC_PAGE_DOWN": "PageDown", "KC_HOME": "Home", "KC_END": "End", "KC_LEFT": "ArrowLeft",
    "KC_RIGHT": "ArrowRight", "KC_UP": "ArrowUp", "KC_DOWN": "ArrowDown",
}
for _i in range(26):
    KC_CHARS[f"KC_{chr(65 + _i)}"] = chr(97 + _i)

# Practice text, by level. The useful knob here is which characters you have to
# reach, not how big your vocabulary is: "numbers" sends you to the Num layer and
# "code" to Sym, which is what makes them harder. Kept lowercase — the page reads
# event.key lowercased, so capitals would be unreachable prompts.
_EASY = """the be to of and a in it is for on with he as you do at but by from they we
or an will my one all would there their what so up out if who get go me when can like
time no just him know take into year your good some see than then now look only come
over back after use two how our work way even new want any give day most us said man
old big small long little own other many first last next best high low right left near
far away again still once here where why while while not all""".split()

_COMMON = _EASY + """because people water number part should between under again around
without always sometimes things place point fact home area book letter house answer study
plant food earth light country music art mother father family friend child school student
power city farm story door group country begin keep find give tell ask seem mean let help
move live play run walk sit stand hear speak buy drink sit need try call turn show follow
carry hold write provide happen bring change close remember grow draw think watch count
""".split()

_HARD = """absolutely according actually although altogether analysis anything appearance
approximately argument assembled associate authority available beautiful beginning believe
completely complicated comprehensive consequently consideration consistent constant
container continuing controlled convenient correction corresponding criticism deliberately
difference difficult disadvantage disappointment discussion distinction effectively
elsewhere emergency emotionally equivalent especially existence experience explanation
extremely frequently government gradually impossible individual industrial intelligence
interesting interpretation involvement knowledge laboratory literature maintenance
manufactured marriage material mathematics meanwhile miserable modification momentum
understanding university usually whether wonderful although language different important
national children example question education community history however something
""".split()

WORDS = {"easy": _EASY, "common": _COMMON, "hard": _HARD}

# Symbol soup for the two top levels; every one of these is checked to be typeable
# on every board by scripts/test_generate_trainer.py.
MARKS = [",", ".", "?", "!", ";", ":", "'", "-"]
CODE = ["->", "==", "!=", "&&", "||", "<", ">", "{", "}", "[", "]", "(", ")", "+", "-",
        "*", "/", "\\", "%", "#", "@", "$", "&", "|", "~", "^", "_", ";", ":", "=", "?", "!"]

LEVELS = {
    "easy": {"list": "easy", "label": "easy words"},
    "common": {"list": "common", "label": "common words"},
    "hard": {"list": "hard", "label": "long words"},
    "punctuation": {"list": "common", "label": "words + punctuation", "marks": MARKS},
    "numbers": {"list": "common", "label": "words + numbers", "marks": MARKS, "numbers": 0.15},
    "code": {"list": "common", "label": "code", "marks": MARKS, "numbers": 0.1, "code": 0.25},
}
for _i in range(10):
    KC_CHARS[f"KC_{_i}"] = str(_i)
for _i in range(1, 25):
    KC_CHARS[f"KC_F{_i}"] = f"F{_i}"

# QMK accepts short and long spellings for the same keycode; the keymap uses both.
KC_CHARS.update({
    "KC_BSPC": "Backspace", "KC_ENT": "Enter", "KC_ESC": "Escape", "KC_SPC": " ",
    "KC_MINS": "-", "KC_EQL": "=", "KC_LBRC": "[", "KC_RBRC": "]", "KC_BSLS": "\\",
    "KC_SCLN": ";", "KC_QUOT": "'", "KC_GRV": "`", "KC_COMM": ",", "KC_SLSH": "/",
    "KC_INS": "Insert", "KC_DEL": "Delete", "KC_PGUP": "PageUp", "KC_PGDN": "PageDown",
    "KC_RGHT": "ArrowRight",
})

ALIAS_RE = re.compile(r"^\s*([A-Z][A-Z0-9_]*)\s*=\s*(.+?)\s*,?\s*$", re.M)
DEFINE_RE = re.compile(r"#define\s+([A-Z][A-Z0-9_]*)\s+(.+)")
FUNC_RE = re.compile(r"^(\w+)\s*\((.*)\)$")
COMBO_ARR_RE = re.compile(r"const\s+uint16_t\s+PROGMEM\s+(\w+)\s*\[\]\s*=\s*\{([^}]*)\}")
COMBO_DEF_RE = re.compile(r"COMBO\((\w+),\s*([^)]+)\)")


def parse_aliases(source):
    """name -> keycode expression, from the enum aliases and #defines in yuanw.c."""
    aliases = dict(DEFINE_RE.findall(source))
    aliases.update(ALIAS_RE.findall(source))
    return aliases


def resolve(token, aliases, depth=0):
    """Browser event.key for a QMK keycode token (following LT/MT/aliases), or None."""
    token = (token or "").strip()
    if not token or depth > 6:
        return None
    if token in aliases:
        return resolve(aliases[token], aliases, depth + 1)
    fn_match = FUNC_RE.match(token)
    if fn_match:
        fn, args = fn_match.group(1), [a.strip() for a in fn_match.group(2).split(",")]
        if fn.startswith(("LT", "MT")) and len(args) > 1:
            return resolve(args[1], aliases, depth + 1)   # LT(layer, tap)
        if fn.endswith("_T") and args:                      # LSFT_T(KC_E)
            return resolve(args[0], aliases, depth + 1)
        return None
    if token.startswith("KC_"):
        return KC_CHARS.get(token)
    return None


def parse_combos(source, token_index):
    """combo output char -> [[key index, ...], ...], positions looked up in the grid."""
    arrays = {name: [t.strip() for t in body.split(",")
                     if t.strip() and t.strip() != "COMBO_END"]
              for name, body in COMBO_ARR_RE.findall(source)}
    combos = {}
    for name, output in COMBO_DEF_RE.findall(source):
        # Only plain keycodes count as typing: an alias such as BSPC_FUN taps out
        # something else entirely (Delete, via shift), so do not claim its name.
        token = output.strip()
        char = KC_CHARS.get(token) if token.startswith("KC_") else None
        if not char:
            continue
        positions = [token_index.get(token) for token in arrays.get(name, [])]
        if not positions or any(p is None for p in positions) or len(positions) < 2:
            continue
        combos.setdefault(char, []).append(positions)
    return combos


# How to reach each layer when no base-layer key annotates it as a hold.
# Source: the LT()/MO() aliases in yuanw.c.
LAYER_HOLD = {
    "Base": None,
    "Sym": "LR DOT / LR G",
    "Nav": "Spc",
    "Win": "Esc",
    "Fun": "BSPC FUN (combo R_NUM+REP_TXT, or K+H one-shot)",
    "Num": "R",
    "Txt": "REP TXT (thumb 0)",
    "Rep": "REP TXT combo / magic key",
    "Pnt": "move the trackball",
}


# process.py writes keymap-drawer icon names in the labels; keep them readable here.
ICONS = {
    "mdi:apple-keyboard-command": "⌘",
    "mdi:apple-keyboard-shift": "⇧",
    "mdi:apple-keyboard-control": "⌃",
    "mdi:apple-keyboard-option": "⌥",
    "mdi:apple-keyboard-caps-lock": "⇪",
    "mdi:keyboard-return": "↵",
    "mdi:backspace-outline": "⌫",
}


def clean(label):
    return re.sub(r"\$\$(.+?)\$\$", lambda m: ICONS.get(m.group(1), ""), str(label or "")).strip()


def typable(label):
    """Browser `event.key` for a display label, or None if unverifiable."""
    if not label:
        return None
    if label in TYPABLE:
        return TYPABLE[label]
    if len(label) == 1 and 33 <= ord(label) <= 126:  # printable ASCII
        return label.lower() if label.isalpha() else label
    return None


def cell_spec(raw):
    if raw is None:
        return {"t": "", "h": "", "s": "", "trans": False}
    if isinstance(raw, dict):
        if raw.get("type") == "trans":
            return {"t": "", "h": "", "s": "", "trans": True}
        return {
            "t": clean(raw.get("t")),
            "h": clean(raw.get("h")),
            "s": clean(raw.get("s")),
            "trans": False,
        }
    return {"t": clean(raw), "h": "", "s": "", "trans": False}


def load_geometry(path, layout_name):
    data = json.load(open(path))
    layouts = data["layouts"]
    if layout_name not in layouts:
        if len(layouts) == 1:
            layout_name = next(iter(layouts))
        else:
            raise SystemExit(f"{path}: no layout {layout_name!r}, has {list(layouts)}")
    return [
        {
            "x": k.get("x", 0),
            "y": k.get("y", 0),
            "w": k.get("w", 1),
            "h": k.get("h", 1),
        }
        for k in layouts[layout_name]["layout"]
    ]


def load_raw_keycodes(path, n_keys):
    """Raw QMK keycode tokens per layer, aligned with the label grid."""
    layers = json.load(open(path))["layers"]
    out = []
    for flat in layers:
        flat = list(flat)[:n_keys]
        out.append(flat + [""] * (n_keys - len(flat)))
    return out


def build_data(target, yaml_path, geom_path, layout_name, qmk_json, firmware):
    keymap = yaml.safe_load(open(yaml_path))
    layers = list(keymap["layers"])  # "0 Base", "1 Sym", ...
    base = layers[0]

    geom = load_geometry(geom_path, layout_name)
    rows = keymap["layers"][base]
    n_keys = sum(len(r) for r in rows)
    # Keep geometry and labels aligned; extra physical keys (unused thumbs,
    # the drawing-only trackball) simply get blank labels.
    geom = geom[:n_keys]

    cells = {}
    for name in layers:
        flat = [k for row in keymap["layers"][name] for k in row][:n_keys]
        flat += [None] * (n_keys - len(flat))
        cells[name] = [cell_spec(k) for k in flat]

    # Raw keycodes tell us what a key taps out (R_NUM -> r, LR_DOT -> .) and
    # what combos fire, neither of which the pretty labels carry.
    raw = load_raw_keycodes(qmk_json, n_keys) if qmk_json else []
    source = Path(firmware).read_text() if firmware and Path(firmware).exists() else ""
    aliases = parse_aliases(source)
    token_index = {}
    for i, token in enumerate(raw[0] if raw else []):
        token_index.setdefault(token, i)
    combos = parse_combos(source, token_index) if raw else {}

    # Which (layer, key) produce each typeable character, base layer first.
    targets = {}
    for li, name in enumerate(layers):
        for i, c in enumerate(cells[name]):
            # The label is authoritative when process.py drew one; otherwise ask
            # the keycode, so layer-tap thumbs (R NUM) and shifted keys still count.
            out = typable(c["t"]) or (resolve(raw[li][i], aliases) if raw else None)
            if out is None:
                continue
            label = c["t"] or (out.upper() if len(out) == 1 else out)
            entry = {
                "char": out,
                "layer": li,
                "keys": [i],
                "label": label,
                "hold": c["h"],
                "shift": c["s"],
                "layerName": name,
            }
            targets.setdefault(out, []).append(entry)
            # Shifted output is its own prompt: ? + " _ are only reachable that way,
            # and a drill that never asks for them teaches half the symbol layer.
            sh = typable(c["s"])
            if sh and sh != out:
                targets.setdefault(sh, []).append(
                    {**entry, "char": sh, "label": c["s"], "shifted": True, "base": c["t"]})

    # Combos are not grid keys, so they stay in their own bucket: {char: [entry]}.
    combo_entries = {
        char: [{
            "char": char,
            "layer": 0,
            "keys": positions,
            "label": char.upper() if len(char) == 1 else char,
            "hold": "",
            "shift": "",
            "layerName": "Combo",
        } for positions in alternatives]
        for char, alternatives in combos.items()
    }

    holds = dict(LAYER_HOLD)
    # Hold key for a layer, taken from the base layer labels when annotated
    # (process.py writes h: "Nav" / "Sym" / "Win" on the layer-tap keys).
    for name in layers:
        short = name.split(" ", 1)[-1]
        for c in cells[base]:
            if c["h"] == short:
                holds[short] = c["t"]
                break

    # What each key taps on the base layer — used to recognise the member
    # presses a combo may leak before it fires, and to find layer keys by name.
    base_char = [
        typable(cells[base][i]["t"]) or (resolve(raw[0][i], aliases) if raw else None)
        for i in range(n_keys)
    ]

    # Physical keys that activate each layer, so the board can light them too.
    hold_keys = {}
    for name in layers[1:]:
        short = name.split(" ", 1)[-1]
        found = [i for i, c in enumerate(cells[base]) if c["h"] == short]
        if not found:
            # Not annotated: fall back to the LAYER_HOLD hint ("R", "REP TXT ...").
            hint = (holds.get(short) or "").split(" ")[0].strip("().").lower()
            if hint:
                found = [i for i, c in enumerate(cells[base])
                         if base_char[i] == hint or c["t"].lower().split(" ")[0] == hint]
        if found:
            hold_keys[name] = found

    # Which hand a key belongs to, from the gap between the halves.
    centre = max(k["x"] + k["w"] / 2 for k in geom) / 2
    hand = [1 if k["x"] + k["w"] / 2 >= centre else 0 for k in geom]

    data = {
        "target": target,
        "layers": layers,
        "keys": geom,
        "cells": cells,
        "targets": targets,
        "holds": holds,
        "holdKeys": hold_keys,
        "baseChar": base_char,
        "hand": hand,
        "combos": combo_entries,
    }
    return data


REPO = Path(__file__).parents[1]
BOARDS = ["charybdis", "dilemma", "imprint", "spankbd"]
INDEX = """<!doctype html><meta charset=utf-8><title>keymap trainers</title>
<style>body{{font:16px ui-sans-serif,system-ui;background:#17181c;color:#e6e6e6;padding:2rem}}
a{{color:#e0a96d}}li{{margin:.3rem 0}}</style>
<h1>keymap trainers</h1><ul>{links}</ul>
"""


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("target", nargs="?")
    ap.add_argument("--index", action="store_true",
                    help="write an index page linking every board instead of a single page")
    ap.add_argument("--yaml", help="default: keymap-drawer/<target>_output.yaml")
    ap.add_argument("--geom", help="default: keymap-drawer/<target>_layout.json")
    ap.add_argument("--layout", help="default: the only layout in --geom")
    ap.add_argument("--out", help="default: keymap-drawer/<target>_trainer.html")
    ap.add_argument("--raw", help="default: keymap-drawer/<target>.json")
    ap.add_argument("--src", default=str(REPO / "yuanw.c"),
                    help="keymap source with the LT()/combo definitions")
    args = ap.parse_args()

    if args.index:
        out = Path(args.out or REPO / "keymap-drawer" / "index.html")
        links = "".join(f'<li><a href="{b}_trainer.html">{b}</a></li>' for b in BOARDS)
        out.parent.mkdir(parents=True, exist_ok=True)
        out.write_text(INDEX.format(links=links))
        print(f"wrote {out}")
        return
    if not args.target:
        ap.error("target is required unless --index")

    # Defaults are all committed files, so the pages can be built without the
    # firmware submodules — that is what lets flake.nix package the site.
    yaml = Path(args.yaml or REPO / "keymap-drawer" / f"{args.target}_output.yaml")
    geom = Path(args.geom or REPO / "keymap-drawer" / f"{args.target}_layout.json")
    out = Path(args.out or REPO / "keymap-drawer" / f"{args.target}_trainer.html")
    raw_json = Path(args.raw or yaml.with_name(f"{args.target}.json"))
    raw_json = raw_json if raw_json.exists() else None
    data = build_data(args.target, yaml, geom, args.layout, raw_json, args.src)
    html = (
        Path(__file__).with_name("trainer_template.html").read_text()
        .replace("__TITLE__", f"{args.target} layout trainer")
        .replace("__DATA__", json.dumps(data, ensure_ascii=False))
        .replace("__WORDS__", json.dumps({"words": WORDS, "levels": LEVELS, "code": CODE},
                                         ensure_ascii=False))
    )
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(html)
    print(f"wrote {out} ({len(data['targets'])} typeable keys, {len(data['layers'])} layers, "
          f"combos: {' '.join(data['combos']) or 'none'})")


if __name__ == "__main__":
    main()
