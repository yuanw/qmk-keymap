# colors
reset  := '\033[0m'
red    := '\033[1;31m'
green  := '\033[1;32m'
yellow := '\033[1;33m'
blue   := '\033[1;34m'
charybdisNS := "bastardkb/charybdis/3x5"
imprintNS := "cyboard/imprint/imprint_letters_only_no_bottom_row"

list:
    @just --list

# build with target parameter
build target:
    #!/usr/bin/env bash
    if [ "{{target}}" = "imprint" ]; then
        just imprint
    elif [ "{{target}}" = "charybdis" ]; then
        just charybdis
    else
        echo "Unknown target: {{target}}"
        exit 1
    fi

# Setup submodule and link directories to submodules
init:
    #!/usr/bin/env bash
    git submodule update --init --recursive --recommend-shallow
    if [ "$(git config submodule.bastardkb-qmk.ignore)" != "all" ]; then
      git config submodule.bastardkb-qmk.ignore all
    fi
    if [ "$(git config submodule.imprint.ignore)" != "all" ]; then
      git config submodule.imprint.ignore all
    fi


# build charybdis/3x5
charybdis:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{justfile_directory()}}/bastardkb-qmk" ]; then
      qmk config user.qmk_home="{{justfile_directory()}}/bastardkb-qmk"
    fi
    qmk compile -c -kb bastardkb/charybdis/3x5 -km yuanw

# build charybdis/3x5
imprint:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{justfile_directory()}}/imprint" ]; then
      qmk config user.qmk_home="{{justfile_directory()}}/imprint"
    fi
    qmk compile -c -kb cyboard/imprint/imprint_letters_only_no_bottom_row -km yuanw



# build imprint
flash_imprint:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{justfile_directory()}}/imprint" ]; then
      qmk config user.qmk_home="{{justfile_directory()}}/imprint"
    fi
    if [ "$(qmk config user.overlay_dir | cut -d '=' -f 2)" != "{{justfile_directory()}}" ]; then
      qmk config user.overlay_dir="{{justfile_directory()}}"
    fi
    qmk flash -c -kb "{{imprintNS}}" -km yuanw

keymap2:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{justfile_directory()}}/bastardkb-qmk" ]; then
      qmk config user.qmk_home="{{justfile_directory()}}/bastardkb-qmk"
    fi
    qmk -v c2json --no-cpp -kb "{{charybdisNS}}" -km yuanw ./keyboards/"{{charybdisNS}}"/keymaps/yuanw/keymap.c > 3x5.json
    keymap parse -c 10 -q 3x5.json > 3x5.yaml
    keymap draw 3x5.yaml -j ./bastardkb-qmk/keyboards/"{{charybdisNS}}"/keyboard.json > 3x5.svg

# keymap
keymap:
    #!/usr/bin/env bash
    qmk -v c2json --no-cpp -kb "{{imprintNS}}" -km yuanw ./keyboards/"{{imprintNS}}"/keymaps/yuanw/keymap.c > imprint.json
    keymap parse -c 10 -q imprint.json > imprint.yaml
    keymap draw imprint.yaml -j ./imprint/keyboards/"{{imprintNS}}"/info.json > imprint.svg
