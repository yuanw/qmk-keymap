#!/usr/bin/env python3
"""Self-check for the trainer generator: run `python scripts/test_generate_trainer.py`."""

import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from generate_trainer import build_data  # noqa: E402

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
    # Anything the browser cannot report as event.key must stay out of the drills.
    for ch in list(t) + list(d["combos"]):
        assert ch.startswith("F") or ch in (
            "Escape", "Backspace", "Tab", "Enter", "Delete", "Insert", "PageUp",
            "PageDown", "Home", "End", "ArrowLeft", "ArrowRight", "ArrowUp", "ArrowDown",
        ) or all(32 <= ord(c) <= 126 for c in ch), f"unmatchable drill target: {ch!r}"
    print(f"{target}: {n} keys, {len(t)} typeable — ok")

print("all good")
