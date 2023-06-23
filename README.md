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

## Customizing
Fetcho is customized using environment variables.
These can be set by adding a line like this to your shells configuration file (eg. .bashrc, .zshrc, config.fish):
```bash
export VARIABLE_NAME="variable value"
```

### `FO_MODULES`
`FO_MODULES` defines the modules that should be shown.
It is a string that contains a list of modules seperated by a space. The modules will be shown from first to last in the variable.

**Valid modules**:

`os`

`kernel`

`uptime`

`shell`

`ram`

`de`

**Default**:
```bash
export FO_MODULES="os kernel uptime shell ram de"
```

### `FO_NERDFONTS`
`FO_NERDFONTS` is used to decide if icons should be shown. If it is set to either `"true"` or `"1"` then icons will be shown. Icons are automatically disabled if you are in a TTY regardless of what this variable is set to.

**Default**:
```bash
export FO_NERDFONTS="false"
```

### `FO_LINETEXT`
`FO_LINETEXT` is the character used for the seperator line between the `username@hostname` and the modules. Make sure it is only 1 character or strange behavior can occur.

**Default**:
```bash
export FO_LINETEXT="─"
```



## Acknowledgments
* [mekb-turtle](https://github.com/mekb-turtle) for helping me with C stuff I didn't understand
* [angelofallars](https://github.com/angelofallars) for creating treefetch, which inspired the design of fetcho (mainly the seperator)
* [dylanaraps](https://github.com/dylanaraps) for pretty much creating fetch programs
