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
    if [ "$(git config submodule.svalboard.ignore)" != "all" ]; then
    git config submodule.svalboard.ignore all
    fi

# Update svalboard git submodule
update-svalboard:
    #!/usr/bin/env bash
    echo "{{ blue }}Updating svalboard submodule...{{ reset }}"
    git submodule update --remote --merge svalboard
    if git diff --quiet svalboard; then
        echo "{{ green }}No svalboard changes to commit{{ reset }}"
    else
        echo "{{ yellow }}Committing svalboard update...{{ reset }}"
        git add svalboard
        git commit -m "Update svalboard submodule"
        echo "{{ green }}Svalboard submodule updated and committed{{ reset }}"
    fi

# Update imprint git submodule
update-imprint:
    #!/usr/bin/env bash
    echo "{{ blue }}Updating imprint submodule...{{ reset }}"
    git submodule update --remote --merge imprint
    if git diff --quiet imprint; then
        echo "{{ green }}No imprint changes to commit{{ reset }}"
    else
        echo "{{ yellow }}Committing imprint update...{{ reset }}"
        git add imprint
        git commit -m "Update imprint submodule"
        echo "{{ green }}Imprint submodule updated and committed{{ reset }}"
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

# Returns the submodule directory for a keyboard
_submodule keyboard:
    #!/usr/bin/env bash
    if [ "{{ keyboard }}" = "imprint" ]; then
      echo "imprint"
    elif [ "{{ keyboard }}" = "charybdis" ]; then
      echo "charybdis"
    else
      printf "{{ red }}Failed: Unknown keyboard: {{ keyboard }}\n"
    fi

# Generate keymap SVG visualization
keymap target:
    #!/usr/bin/env bash
    just setup {{ target }}
    kb=$(just _keyboard {{ target }})
    submod=$(just _submodule {{ target }})
    outdir="keymap-drawer"

    # Use yuanw.c directly (contains the keymaps array)
    qmk -v c2json --no-cpp -kb "$kb" -km yuanw ./yuanw.c > $outdir/{{ target }}.json

    if [ "{{ target }}" = "imprint" ]; then
      # Expand LAYOUT_LR (35 keys) to LAYOUT_let_no_bottom_row (48 keys)
      python $outdir/expand_layout.py $outdir/{{ target }}.json
      KEYMAP_raw_binding_map='{"&bootloader": "BOOT"}' keymap parse -c 10 -q $outdir/{{ target }}.json > $outdir/{{ target }}.yaml
      python $outdir/process.py $outdir/{{ target }}.yaml $outdir/{{ target }}_output.yaml
      keymap draw $outdir/{{ target }}_output.yaml -j ./$submod/keyboards/$kb/info.json > $outdir/{{ target }}.svg
      python $outdir/hide_empty_keys.py $outdir/{{ target }}.svg
    elif [ "{{ target }}" = "charybdis" ]; then
      # Charybdis LAYOUT_LR maps 1:1 to LAYOUT, just fix the layout name
      sed -i 's/"LAYOUT_LR"/"LAYOUT"/' $outdir/{{ target }}.json
      KEYMAP_raw_binding_map='{"&bootloader": "BOOT"}' keymap parse -c 10 -q $outdir/{{ target }}.json > $outdir/{{ target }}.yaml
      python $outdir/process.py $outdir/{{ target }}.yaml $outdir/{{ target }}_output.yaml
      keymap draw $outdir/{{ target }}_output.yaml -j ./$submod/keyboards/$kb/keyboard.json > $outdir/{{ target }}.svg
    fi
    echo "{{ green }}Generated $outdir/{{ target }}.svg{{ reset }}"

# Generate compile_commands.json for clangd LSP
compiledb target:
    #!/usr/bin/env bash
    just setup {{ target }}
    qmk compile --compiledb -kb $(just _keyboard {{ target }}) -km yuanw
    cp {{ target }}/compile_commands.json .

# Format C files under keyboards directory
format:
    find {{ justfile_directory() }}/keyboards -name '*.c' -o -name '*.h' | xargs clang-format -i
    find {{ justfile_directory() }} -maxdepth 1 \( -name '*.c' -o -name '*.h' \) | xargs clang-format -i
