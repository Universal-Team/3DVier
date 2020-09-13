# DSVier
A Connect Four clone for Nintendo DS(i).

## Compilation

To build DSVier from source, you need devkitPro installed, along with devkitARM, libnds and libfat. To get these you can run `pacman -S nds-dev --noconfirm`. You will also need [bmp2ds](https://github.com/Universal-Team/bmp2ds/releases)'s latest release inside the `graphics` directory, to build the `.gfx` files for DSVier.

Now that you have everything installed, you just need to clone the repository and run `make nds` in the root directory, or directly run `make` in the nds directory. You will find your `DSVier.nds` build at the end inside the `builds` folder from the root directory.

## Credits
### Main Developers
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): App Idea and Main Developer.

## Translators
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): English.

## Others
- [devkitPro](https://github.com/devkitPro), [Wintermute](https://github.com/WinterMute): For devkitARM, Libnds and Libfat.