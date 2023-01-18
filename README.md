# ModLoader

Mod loader for Tomb Raider games running on cdcEngine, allows loading files from mods folder without modifying the bigfiles.
This code is orginally part of [TRAE-Menu-Hook](https://github.com/TheIndra55/TRAE-menu-hook) but available as standalone without all other functionality.

Currently the following games are supported:

- Tomb Raider: Legend
- Tomb Raider: Anniversary
- Tomb Raider: Underworld

If you get an error message about your version not being compatible please open an issue.

## Usage

1. Head to the releases page and download the latest version
2. Unzip all files in your game folder, overwriting any existing files.

## Build

First clone the repository with submodules

```bash
git clone https://github.com/TombRaiderModding/ModLoader.git 
git submodule init
git submodule update
```

Also make sure you have installed [premake5](https://premake.github.io/) on your system.

Run `premake5 vs2022` in the cloned folder, now open the generated generated solution (.sln) and start building.

## License

This code is licensed under GPL-3.0, the license can be found in the `LICENSE` file.