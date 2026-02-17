{
  description = "Hyprwatch - Time tracking daemon and client for Hyprland";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "hyprwatch";
          version = "0.0.1";
          src = self;
          
          nativeBuildInputs = with pkgs; [
            cmake
          ];
          
          buildInputs = with pkgs; [
            ncurses
          ];
          
          cmakeFlags = [
            "-DCMAKE_INSTALL_PREFIX=${placeholder "out"}"
          ];
        };

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = with pkgs; [
            cmake
            gcc
          ];
          buildInputs = with pkgs; [
            ncurses
          ];
        };
      }
    );
}
