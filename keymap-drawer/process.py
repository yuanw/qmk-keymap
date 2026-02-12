import sys
from yaml import load, dump
from collections import namedtuple
try:
    from yaml import CLoader as Loader, CDumper as Dumper
except ImportError:
    from yaml import Loader, Dumper

Point = namedtuple('Point', ['layer', 'row', 'key'])
# hardcode layer list
# Layer order from yuanw.c: enum layers { BASE, SYM, NAV, WIN, PNT, NUM, TXT, FUN, REP }
layer_dict = {
    "L0": "0 Base",
    "L1": "1 Sym",
    "L2": "2 Nav",
    "L3": "3 Win",
    "L4": "4 PNT",
    "L5": "5 Num",
    "L6": "6 Txt",
    "L7": "7 Fun",
    "L8": "8 Rep"}

layer_key_dict = {
    "LLAYER_POINTER": "Cursor",
    "LLAYER_MEDIA": "Med",
    "LLAYER_NAVIGATION": "Nav",
    "LLAYER_FUNCTION": "Fun",
    "LLAYER_NUMERAL": "Num",
    "LLAYER_SYMBOLS": "Sym",
}

key_dict = {
    "=": {"t": "=", "s": "+"},
    "-": {"t": "-", "s": "_"},
    "'": {"t": "'", "s": "\""},
    "/": {"t": "/", "s": "?"},
    "HRM C": {"t": "C", "h": "Meta"},
    "HRM S": {"t": "S", "h": "Meta"},

    "HRM E":{"t": "E", "h": "$$mdi:apple-keyboard-control$$", "type":"ghost"},
    "HRM T":{"t": "T", "h": "$$mdi:apple-keyboard-control$$"},

    "HRM A": {"t": "A", "h": "$$mdi:apple-keyboard-shift$$ "},
    "HRM H": {"t": "H", "h": "$$mdi:apple-keyboard-shift$$", "type": "ghost"},
    "HRM I": {"t": "I", "h": "$$mdi:apple-keyboard-option$$"},
    "HRM N": {"t": "N", "h": "$$mdi:apple-keyboard-option$$"},

     #apple-keyboard-command apple-keyboard-shift"
     #apple-keyboard-option apple-keyboard-control"
    "ESC WIN": {"t": "Esc", "s": "Enter", "h": "Win" },
    "SPC NAV": {"t": "Spc", "s": "Tab", "h": "Nav", "type": "ghost"},
    "LR_DOT":  {"t": ".", "h": "Sym" , "s": ">"},
    "LR_G":  {"t": "G", "h": "Sym" },
    ",":  {"t": ",","s": "<"},
    ".": {"t":".", "s": ">"},
    ";": {"t":";", "s": ":"},
    # "ESC": "\u241B",
    "BSPC": "\u232B",
    "VOLD": "🔉",
    "VOLU": "🔊",
    "MUTE": "\U0001F507",
    "MNXT": "\u21E5",
    "MPRV": "\u21E4",
    "LEFT": "\u25C1",
    "RGHT": "\u25B7",
    "DOWN": "Down",
    "QK REP": "\u21BB",
    "QK AREP": "*",
    "ALTREP2": "\u21BA 2",
    "ALTREP3": "\u21BA 3",
    "UP": "\u25B3",
    "QK LLCK": "$$mdi:lock$$",
    "ENT": "⏎",
    "LGUI": "⌘",
    "RGUI": "⌘",
    "LALT": "⌥",
    "LSFT": "⇧",
     "RSFT": "⇧",
    "LCTL" : "⌃",
     "RCTL": "⌃",
    "CAPS": "⇪",
    "QK CAPS WORD TOGGLE": "Caps Word",
    "Alt+Gui+1":"⌘⌥1",
    "Alt+Gui+2":"⌘⌥2",
    "Alt+Gui+3":"⌘⌥3",
    "LAG(KC_4)":"⌘⌥4",
    "LSG(KC_1)": "⌘⇧1",
    "LSG(KC_2)": "⌘⇧2",
    "LSG(KC_3)": "⌘⇧3",
    "LSG(KC_4)": "⌘⇧4",
}

def main():
    input_file = sys.argv[1] if len(sys.argv) > 1 else 'imprint.yaml'
    output_file = sys.argv[2] if len(sys.argv) > 2 else 'output.yaml'

    layer_hold_dict = {}
    new_data = {}
    with open(input_file, encoding="utf8") as reader:
        data = load(reader, Loader=Loader)
        new_data['layers'] = {}
        for l in data.get('layers').keys():
            print(l)
            new_data['layers'][layer_dict[l]] = []
            for row_counter, row in enumerate(data.get('layers')[l]):
                new_row = []
                for key_counter, k in enumerate(row):
                    # add held for layer activation
                    # since this is miryoku-inspired
                    p = Point(layer_dict[l], row_counter, key_counter)
                    if p in layer_hold_dict:
                        new_row.append({'type':'held'})
                    elif isinstance(k, str):
                        new_row.append(key_dict.get(k, k))
                    elif isinstance(k, dict):
                        old_v = k.get('h','')
                        # add layer hold entry, so we can add svg held class
                        # highlight correctly
                        if old_v in layer_key_dict:
                            p = Point(layer_key_dict[old_v], row_counter, key_counter)
                            layer_hold_dict[p] = layer_dict[l]

                        k['h'] = key_dict.get(old_v, old_v)
                        if 't' in k:
                            old_v = k.get('t','')
                            k['t'] = key_dict.get(old_v, old_v)
                        new_row.append(k)
                    else:
                        new_row.append(k)
                new_data['layers'][layer_dict[l]].append(new_row)

    with open(output_file, "w", encoding="utf8") as writer:
        dump(new_data, writer)

if __name__ == "__main__":
    main()
