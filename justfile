# colors

reset := '\033[0m'
red := '\033[1;31m'
green := '\033[1;32m'
yellow := '\033[1;33m'
blue := '\033[1;34m'
charybdisNS := "bastardkb/charybdis/3x5"
imprintNS := "cyboard/imprint/imprint_letters_only_no_bottom_row"

list:
    @just --list

# build with target parameter
build target:
    #!/usr/bin/env bash
    just setup {{ target }}
    qmk compile -kb $(just _keyboard {{ target }}) -km yuanw

flash target:
    #!/usr/bin/env bash
    just setup {{ target }}
    qmk flash -kb $(just _keyboard {{ target }}) -km yuanw


# Setup submodule and link directories to submodules
init:
    #!/usr/bin/env bash
    git submodule update --init --recursive --recommend-shallow
    if [ "$(git config submodule.charybdis.ignore)" != "all" ]; then
    git config submodule.charybdis.ignore all
    fi
    if [ "$(git config submodule.imprint.ignore)" != "all" ]; then
    git config submodule.imprint.ignore all
    fi

# Update git submodules and commit the changes
update-submodules:
    #!/usr/bin/env bash
    echo "{{ blue }}Updating git submodules...{{ reset }}"
    git submodule update --remote --merge

    # Check if any submodules have changes
    if git diff --quiet && git diff --staged --quiet; then
        echo "{{ green }}No submodule changes to commit{{ reset }}"
    else
        echo "{{ yellow }}Committing submodule updates...{{ reset }}"

        # Get list of changed submodules
        changed_submodules=$(git diff --name-only)
        if [ -n "$changed_submodules" ]; then
            echo "{{ blue }}Changed submodules:{{ reset }}"
            echo "$changed_submodules"
            git add $changed_submodules
        fi

        git commit -m "Update git submodules"
        echo "{{ green }}Submodules updated and committed{{ reset }}"
    fi

setup target:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{ justfile_directory() }}/{{ target }}" ]; then
    qmk config user.qmk_home="{{ justfile_directory() }}/{{ target }}"
    fi
    if [ "$(qmk config user.overlay_dir | cut -d '=' -f 2)" != "{{ justfile_directory() }}" ]; then
    qmk config user.overlay_dir="{{ justfile_directory() }}"
    fi

_keyboard keyboard:
    #!/usr/bin/env bash
    if [ "{{ keyboard }}" = "imprint" ]; then
      echo "{{ imprintNS }}"
    elif [ "{{ keyboard }}" = "charybdis" ]; then
      echo "{{ charybdisNS }}"
    else
      printf "{{ red }}Failed: Unknown keyboard: {{ keyboard }}\n"
    fi

keymap2:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{ justfile_directory() }}/bastardkb-qmk" ]; then
    qmk config user.qmk_home="{{ justfile_directory() }}/bastardkb-qmk"
    fi
    qmk -v c2json --no-cpp -kb "{{ charybdisNS }}" -km yuanw ./keyboards/"{{ charybdisNS }}"/keymaps/yuanw/keymap.c > 3x5.json
    keymap parse -c 10 -q 3x5.json > 3x5.yaml
    keymap draw 3x5.yaml -j ./bastardkb-qmk/keyboards/"{{ charybdisNS }}"/keyboard.json > 3x5.svg

# keymap
keymap:
    #!/usr/bin/env bash
    just setup imprint
    qmk -v c2json --no-cpp -kb "{{ imprintNS }}" -km yuanw ./keyboards/"{{ imprintNS }}"/keymaps/yuanw/keymap.c > imprint.json
    KEYMAP_raw_binding_map='{"&bootloader": "BOOT"}' keymap parse -c 10 -q imprint.json > imprint.yaml
    python process.py
    keymap draw output.yaml -j ./imprint/keyboards/"{{ imprintNS }}"/info.json > imprint.svg
    python hide_empty_keys.py imprint.svg

# Format C files under keyboards directory
c-format:
    find {{ justfile_directory() }}/keyboards -name '*.c' -o -name '*.h' | xargs clang-format -i
