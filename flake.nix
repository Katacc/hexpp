{
  description = "Cpp development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      pkgs = import nixpkgs {
        system = "x86_64-linux";
      };
    in
    {
      devShells."x86_64-linux".default = pkgs.mkShell {
        buildInputs = with pkgs; [
          ninja
          clang-tools
          clang
          gcc
          zip
          unzip
          cmake
          cmakeCurses

          libcxxStdenv
          glibc.static
          zlib.static
          ocaml
        ];

        shellHook = ''

          # Set values for your system
          export VCPKG_ROOT=/home/$USER/vcpkg
          export HXPP_LOCATION=/home/$USER/bin/hxpp

          export PATH=$VCPKG_ROOT:$PATH
          export PATH=$HXPP_LOCATION:$PATH

          # Compilers
          export CXX_COMPILER_PATH=g++
          export C_COMPILER_PATH=gcc
          export C_DEBUGGER_PATH=gdb

          echo "CPP dev environment!"
          echo "hx++ location: $HXPP_LOCATION"

        '';
      };
    };
}
