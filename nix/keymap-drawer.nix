{ lib
, buildPythonApplication
, buildPythonPackage
, callPackage
, fetchFromGitHub
, setuptools
, poetry-core
, pydantic
, pyparsing
, pcpp
, pyyaml
, platformdirs
, pydantic-settings
, tree-sitter
}:
let
  tree-sitter-devicetree = callPackage ./tree-sitter-devicetree.nix { };
in
buildPythonApplication rec {
  pname = "keymap-drawer";
  version = "0.22.0";
  pyproject = true;

  src = fetchFromGitHub {
    owner = "caksoylar";
    repo = pname;
    rev = "f9cf9d3677368a7fb1dc505c51b8156fafa66fe2";
    sha256 = lib.fakeSha256;
    #sha256 = "sha256-EfFCuqL2/eMUweMip+YUa4iCZouqKC3PbiKRWcLxCvg=";
    # rev = "855933863fcc6f0db6098a03e679319dbf7f8bf2";
    #                      sha256 = lib.fakeSha256;
    #hash = "sha256-InddS9NxVrYOufiP7iWQTQ3VBeJgX2UlBA+Gf7ZfFrI=";
  };

  build-system = [ poetry-core ];

  propagatedBuildInputs = [
    pydantic
    pcpp
    pyyaml
    platformdirs
    pydantic-settings
    pyparsing
    tree-sitter
    tree-sitter-devicetree
  ];

  doCheck = false;

  meta = {
    homepage = "https://github.com/caksoylar/keymap-drawer";
    description = "Parse QMK & ZMK keymaps and draw them as vector graphics";
    license = lib.licenses.mit;
  };
}
