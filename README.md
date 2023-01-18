# ModLoader

Mod loader for Tomb Raider games running on cdcEngine, allows loading files from mods folder without modifying the bigfiles.
This code is orginally part of [TRAE-Menu-Hook](https://github.com/TheIndra55/TRAE-menu-hook) but available as standalone without all other functionality.

Currently the following games are supported:

- Tomb Raider: Legend
- Tomb Raider: Anniversary
- Tomb Raider: Underworld

If you get an error message about your version not being compatible please open an issue.

## Usage

1. Head to the [Releases](https://github.com/TombRaiderModding/ModLoader/releases) page and download the latest version
2. Unzip all files in your game folder, overwriting any existing files.
3. Drop any files or mods in the mods folder.

## Mods folder

The mods folder is located in the game directory, any file placed in this folder will be loaded instead of the file in the Bigfile.

For files in the PC-W folder the PC-W prefix has been omitted, so pc-w\lara.drm will become mods\lara.drm. Other locations such as \trae\pc\objectlist.txt have been located in the mods folder.

Localized files can be done by appending the specialization mask after the filename such as locals.bin_001 for English and locals.bin_002 for German. The highest bit like for Next Generation Content needs to be omitted.

## Build

Install [premake5](https://premake.github.io/) and clone the repository. After invoke premake5 to create the solution files.

```bash
git clone --recurse-submodules https://github.com/TombRaiderModding/ModLoader.git
cd ModLoader
premake5 vs2022
```

Now open the generated generated solution (.sln) and start building.

## License

This code is licensed under GPL-3.0, the license can be found in the `LICENSE` file.