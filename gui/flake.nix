{
    description = "C basic flake";

    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    };

    outputs = {self, nixpkgs}: let 
        system = "x86_64-linux";
        pkgs = import nixpkgs {inherit system;};
    in {
        devShells.${system}.default = pkgs.mkShell {
            name = "C";

            packages = with pkgs; [
                python313
                python313Packages.pyserial
                python313Packages.tkinter
            ];
        };
    };
}
