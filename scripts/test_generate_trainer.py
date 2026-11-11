#!/usr/bin/env python3
"""Self-check for the trainer generator: run `python scripts/test_generate_trainer.py`."""

import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from generate_trainer import build_data, WORDS, LEVELS, CODE  # noqa: E402

ROOT = Path(__file__).parents[1]
GEOM = {
    "imprint": ("keymap-drawer/imprint_layout.json", "LAYOUT_let_no_bottom_row"),
    "charybdis": ("charybdis/keyboards/bastardkb/charybdis/3x5/info.json", "LAYOUT"),
    "dilemma": ("charybdis/keyboards/bastardkb/dilemma/3x5_3_trackball/keyboard.json",
                "LAYOUT_split_3x5_3"),
    "spankbd": ("holykeebs_qmk/keyboards/holykeebs/spankbd/keyboard.json",
                "LAYOUT_split_3x5_3"),
}

for target, (geom, layout) in GEOM.items():
    ypath = ROOT / "keymap-drawer" / f"{target}_output.yaml"
    if not ypath.exists():
        print(f"skip {target} (run `just keymap {target}` first)")
        continue
    d = build_data(target, ypath, ROOT / geom, layout,
                   ROOT / "keymap-drawer" / f"{target}.json", ROOT / "yuanw.c")
    n = len(d["keys"])
    assert len(d["layers"]) == 9, d["layers"]
    assert n > 30, n
    for name, cells in d["cells"].items():
        assert len(cells) == n, f"{target}/{name}: {len(cells)} cells vs {n} keys"

    t = d["targets"]
    assert t["q"][0]["layer"] == 0, "q should sit on the base layer"
    assert t["q"][0]["char"] == "q"
    assert "%" in t and t["%"][0]["layerName"] == "1 Sym", "symbols come from the Sym layer"
    assert t["5"][0]["layerName"] == "6 Num" and d["holds"]["Num"] == "R"
    assert d["holds"]["Sym"] == ".", "Sym hold comes from the LR DOT label"
    # The tap of a layer-tap thumb (r = tap R_NUM) must be drillable on base.
    assert "r" in t and t["r"][0]["layer"] == 0, "r comes from the R_NUM thumb tap"
    assert d["baseChar"][t["r"][0]["keys"][0]] == "r"
    # b and w are combos, not keys: ; + . and m + g.
    assert "b" in d["combos"] and "w" in d["combos"], d["combos"].keys()
    assert len(d["combos"]["b"][0]["keys"]) == 2
    assert all(len(e["keys"]) > 1 for v in d["combos"].values() for e in v)
    # Layer keys to light up while holding, and which hand each key sits under.
    sym = d["holdKeys"]["1 Sym"]
    assert len(sym) == 2, sym                      # '.' and 'G' both reach SYM
    assert d["hand"][sym[0]] != d["hand"][sym[1]], "reach SYM with either hand"
    assert d["holdKeys"]["6 Num"] == [t["r"][0]["keys"][0]]
    assert set(d["hand"]) == {0, 1}
    # Shifted output has to be its own prompt, or the page never asks for ? + " _
    for ch in '?+"_':
        assert ch in t, f"{target}: shifted {ch!r} is not drillable"
        assert any(e["shifted"] for e in t[ch]), f"{target}: {ch!r} has no shift route"
    # Practice text must be reachable: the page skips characters the keymap
    # cannot produce, so an unreachable prompt would just vanish silently.
    reach = set(t) | set(d["combos"])
    letters = {c for words in WORDS.values() for word in words for c in word}
    assert not letters - reach, f"{target}: no key types {sorted(letters - reach)}"
    for name, lv in LEVELS.items():
        assert lv["list"] in WORDS, name
        for c in lv.get("marks", []):
            assert c in reach, f"{target}: punctuation {c!r} unreachable in {name}"
    for token in CODE + [str(i) for i in range(10)]:
        assert set(token) <= reach, f"{target}: {token!r} unreachable in the code level"
    # Anything the browser cannot report as event.key must stay out of the drills.
    for ch in list(t) + list(d["combos"]):
        assert ch.startswith("F") or ch in (
            "Escape", "Backspace", "Tab", "Enter", "Delete", "Insert", "PageUp",
            "PageDown", "Home", "End", "ArrowLeft", "ArrowRight", "ArrowUp", "ArrowDown",
        ) or all(32 <= ord(c) <= 126 for c in ch), f"unmatchable drill target: {ch!r}"
    print(f"{target}: {n} keys, {len(t)} typeable — ok")

# keymap-drawer/<target>_layout.json is what flake.nix builds the pages against
# (a flake source carries no submodules), so keep the copies honest.
for target, (geom, layout) in GEOM.items():
    committed = ROOT / "keymap-drawer" / f"{target}_layout.json"
    assert committed.exists(), f"{target}: no {committed.name}, run `just layouts`"
    mine = json.load(open(committed))["layouts"][layout]["layout"]
    assert len(mine) >= 30, f"{target}: {committed.name} looks empty"
    src = ROOT / geom
    if src.exists():
        assert mine == json.load(open(src))["layouts"][layout]["layout"], \
            f"{target}: {committed.name} is stale, run `just layouts`"

print("all good")
