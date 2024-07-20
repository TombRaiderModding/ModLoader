# ModLoader

Mod loader for Tomb Raider games running on cdcEngine, allows loading files from mods folder without modifying the bigfiles.
This code is orginally part of [TRLAU-Menu-Hook](https://github.com/TheIndra55/TRLAU-menu-hook) but available standalone without all other functionality.

Currently the following games are supported:

- Tomb Raider: Legend
- Tomb Raider: Anniversary
- Tomb Raider: Underworld
- Lara Croft and the Guardian of Light

If you get an error message about your version not being compatible please open an issue.

## Installation

1. Head to the [Releases](https://github.com/TombRaiderModding/ModLoader/releases) page and download the latest version
2. Unzip all files in your game folder, overwriting any existing files.
3. Drop any files or mods in the `mods` folder.

## Build

Install [premake5](https://premake.github.io/) and clone the repository. Next invoke premake5 to create the solution files.

```bash
git clone --recurse-submodules https://github.com/TombRaiderModding/ModLoader.git
cd ModLoader
premake5 vs2022
```

Now open the generated generated solution (.sln) and build.

## License

This code is licensed under GPL-3.0, the license can be found in the `LICENSE` file.