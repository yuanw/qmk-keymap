# colors
reset  := '\033[0m'
red    := '\033[1;31m'
green  := '\033[1;32m'
yellow := '\033[1;33m'
blue   := '\033[1;34m'

# alias b := build
# alias f := flash
# alias l := layout
# alias w := watch-layout

list:
    @just --list

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

# build imprint
imprint:
    #!/usr/bin/env bash
    if [ "$(qmk config user.qmk_home | cut -d '=' -f 2)" != "{{justfile_directory()}}/imprint" ]; then
      qmk config user.qmk_home="{{justfile_directory()}}/imprint"
    fi
    if [ "$(qmk config user.overlay_dir | cut -d '=' -f 2)" != "{{justfile_directory()}}" ]; then
      qmk config user.overlay_dir="{{justfile_directory()}}"
    fi
    qmk compile -c -kb cyboard/imprint/imprint_letters_only_no_bottom_row -km yuanw
