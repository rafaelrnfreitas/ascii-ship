# ASCII SHIP

A small arcade space shooter written in C, rendered by a custom software 3D renderer on top of [SDL3](https://github.com/libsdl-org/SDL). Fly your ship, shoot down endless waves of enemies that get harder over time, and put your initials on the leaderboard.

**Demo video:** [Watch on YouTube](https://youtu.be/9Z6Daq8gJuk) ([or download the file](docs/video-ascii-ship.mp4))

## Features

- Software renderer with meshes, lighting, and a built-in bitmap font (no external assets)
- Enemy AI state machine: drifters, chasers, and shooters that fire telegraphed bursts
- Endless waves with rising difficulty
- Lives with invulnerability blinking, screen shake, and muzzle flashes
- Local leaderboard (top scores saved to `scores.txt`)

## Controls

| Key | Action |
| --- | --- |
| Arrow keys | Move the ship |
| Space (hold) | Fire |
| Up / Down (or W / S) | Move in menus |
| Enter | Confirm |
| Left / Right (or A / D) | Select a letter when entering initials |
| R | Restart after game over |
| Esc | Quit (back out of the leaderboard) |

## Requirements

You need:

- A C compiler (Clang or GCC)
- [CMake](https://cmake.org/) 3.16 or newer
- Git (SDL3 is pulled in as a submodule)
- A system able to open a window (SDL3 is built from source with the project)

### macOS

```sh
xcode-select --install   # Clang and system frameworks
brew install cmake git
```

### Debian / Ubuntu

```sh
sudo apt update
sudo apt install build-essential cmake git pkg-config \
    libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxi-dev \
    libxss-dev libwayland-dev libxkbcommon-dev libasound2-dev \
    libpulse-dev libdrm-dev libgbm-dev libgl1-mesa-dev libegl1-mesa-dev
```

On other distributions, install the equivalents. See the [SDL build docs](https://github.com/libsdl-org/SDL/blob/main/docs/README-linux.md) for the full list.

> Windows is not configured (the build links `m` directly). Use WSL2 or adapt `CMakeLists.txt`.

## Build and run

Clone with submodules (SDL lives in `lib/SDL`):

```sh
git clone --recurse-submodules https://github.com/rafaelrnfreitas/game.git
cd game
```

If you already cloned without `--recurse-submodules`:

```sh
git submodule update --init
```

Configure, build, and run:

```sh
cmake -S . -B build
cmake --build build
./build/game
```

The first build also compiles SDL3 and takes a bit longer. Speed it up with `cmake --build build -j`.

For an optimized build:

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

To start over from scratch, delete the build directory: `rm -rf build`.

## Project layout

```
src/
  main.c            Entry point
  engine/           Window, main loop, input, renderer, screens (menu, game over)
  game/             Player, enemies, AI, waves, lasers, lives, leaderboard, scene
  graphics/         Meshes, camera, lights, color, bitmap font
  math/             vec2/vec3/vec4/mat4
include/            Headers (mirrors src/)
lib/SDL/            SDL3 (git submodule)
docs/               Demo video
```

Gameplay tuning (enemy speeds, fire rates, lives, ...) lives in `include/game/tuning.h`.

## Troubleshooting

- **`lib/SDL` is empty or CMake cannot find SDL:** run `git submodule update --init`.
- **Missing X11/Wayland/audio headers on Linux:** install the packages from the Debian/Ubuntu list above.
- **Stale configuration after changing branches or compilers:** `rm -rf build` and configure again.

## License

GNU GPL v3. See [LICENSE](LICENSE).
