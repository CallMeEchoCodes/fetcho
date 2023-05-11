<div align='center'>

# Fetcho
A simple fetch made in `C++` <br /> <br />
![Image](https://i.imgur.com/o58uibP.png) <br /> <br />
![Release](https://img.shields.io/github/v/release/CallMeEchoCodes/fetcho?display_name=tag&style=for-the-badge&labelColor=181825&color=89dceb)
![Stars](https://img.shields.io/github/stars/CallMeEchoCodes/fetcho?style=for-the-badge&labelColor=181825&color=f9e2af)
![License](https://img.shields.io/github/license/CallMeEchoCodes/fetcho?style=for-the-badge&labelColor=181825&color=f5c2e7)

</div>

## Installing
### Arch-based
If you use an AUR helper, use it instead, e.g `paru -S fetcho` or `paru -S fetcho-git`
```bash
git clone https://aur.archlinux.org/fetcho.git # use fetcho-git.git instead for latest commit
cd foto
makepkg -si
```

### Debian-based
A .deb package is planned for an easier installation
```bash
sudo apt install build-essential libprocps-dev
git clone https://github.com/CallMeEchoCodes/fetcho.git
cd foto
git checkout "$(git describe --tags --abbrev=0)" # checkout to latest tag, omit for latest commit
sudo make install RELEASE=1
```

### Other distros
Install `libprocps` (also known as `procps-ng`) using your package manager before you do this.

```bash
git clone https://github.com/CallMeEchoCodes/fetcho.git
cd fetcho
git checkout "$(git describe --tags --abbrev=0)" # checkout to latest tag, omit for latest commit
sudo make install RELEASE=1
```

Specify `INSTALL_DIR` in `make install` to specify where to install to, read `Makefile` for more information

## Acknowledgments
* [mekb-turtle](https://github.com/mekb-turtle) for helping me with C stuff I didn't understand
* [angelofallars](https://github.com/angelofallars) for creating treefetch, which inspired the design of fetcho (mainly the seperator)
* [dylanaraps](https://github.com/dylanaraps) for pretty much creating fetch programs
