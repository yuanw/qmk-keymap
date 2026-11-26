{
  description = "my keeb";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-parts.url = "github:hercules-ci/flake-parts";
    flake-root.url = "github:srid/flake-root";
    treefmt-nix.url = "github:numtide/treefmt-nix";
    treefmt-nix.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = [
        "aarch64-darwin"
        "aarch64-linux"
        "x86_64-darwin"
        "x86_64-linux"
      ];
      imports = [
        # inputs.haskell-flake.flakeModule
        inputs.treefmt-nix.flakeModule
        inputs.flake-root.flakeModule
      ];
      perSystem =
        { pkgs
        , lib
        , config
        , system
        , ...
        }:

        {
          packages.drawer = pkgs.python3Packages.callPackage ./nix/keymap-drawer.nix { };

          # The trainer site, buildable anywhere: nix build .#trainer && ./result/bin/keymap-trainer [target]
          # Every input it reads is committed under keymap-drawer/ — a flake source
          # carries no git submodules, so the firmware trees are not available here.
          packages.trainer =
            let
              targets = [ "charybdis" "dilemma" "imprint" "spankbd" ];
              python3 = pkgs.python3.withPackages (ps: [ ps.pyyaml ]);
              gen = t: ''
                python3 ${./scripts}/generate_trainer.py ${t} --yaml ${./keymap-drawer}/${t}_output.yaml --geom ${./keymap-drawer}/${t}_layout.json --raw ${./keymap-drawer}/${t}.json --src ${./yuanw.c} --out $out/pages/${t}_trainer.html
              '';
              pages = pkgs.runCommand "keymap-trainer-pages" { nativeBuildInputs = [ python3 ]; } ''
                mkdir -p $out/pages
                ${lib.concatStrings (map gen targets)}
                python3 ${./scripts}/generate_trainer.py --index --out $out/pages/index.html
              '';
            in
            pkgs.writeShellApplication {
              name = "keymap-trainer";
              runtimeInputs = [ python3 ];
              text = ''
                # keymap-trainer [target|--stop] — serve the pages on 127.0.0.1 (no target: the index)
                if [ "''${1:-}" = --stop ]; then
                  exec python3 ${./scripts}/serve_trainer.py --stop
                fi
                if [ $# -gt 0 ]; then page="''${1}_trainer.html"; else page=index.html; fi
                exec env TRAINER_ROOT=${pages}/pages python3 ${./scripts}/serve_trainer.py "$page"
              '';
            };

          treefmt.config = {
            inherit (config.flake-root) projectRootFile;
            package = pkgs.treefmt;
            programs.nixpkgs-fmt.enable = true;
            programs.clang-format.enable = true;
          };

          packages.draw = pkgs.writeShellApplication {
            name = "reiryoku-draw";
            runtimeInputs = with pkgs;
              [
                config.packages.drawer
                (python3.withPackages (ps: [ ps.pyyaml ]))
              ];
            text = ''
              keymap parse -c 10 -q ${config.packages.firmware}/share/reiryoku.json  > reiryoku.yaml
               sed -i -E "s/LAYOUT_charybdis_3x5/LAYOUT/g" reiryoku.yaml
              keymap draw reiryoku.yaml > reiryoku.svg
               python process.py
               keymap draw output.yaml > reiryoku.svg
            '';
          };

          # Default shell.
          devShells.default = pkgs.mkShell {
            buildInputs = with pkgs; [
              clang-tools
              (python3.withPackages (ps: [ ps.pyyaml ps.beautifulsoup4 ps.lxml ]))
              config.packages.drawer
              qmk
              just
            ];
            #https://github.com/EdenEast/qmk/blob/main/flake.nix
            shellHook = ''
              # Prevent the avr-gcc wrapper from picking up host GCC flags
              # like -iframework, which is problematic on Darwin
              unset NIX_CFLAGS_COMPILE_FOR_TARGET
            '';

            # See https://haskell.flake.page/devshell#composing-devshells
            inputsFrom = [
              config.flake-root.devShell
              config.treefmt.build.devShell
            ];
          };
        };
    };
}
