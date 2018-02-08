- [Building on Linux](#building-on-linux)
- [Building on Mac](#building-on-max)
- [Building on Windows](#building-on-windows)

## Building on Linux

I build the Linux edition
using [Travis CI](https://travis-ci.org).

- The `.travis.yml` file configures the build.

From [the VCVRack README](https://github.com/vcvrack/rack):

> With your distro's package manager,
  make sure you have installed `gcc`, `make`, `cmake`, `tar`, and `unzip`.

## Building on Mac

I build the Mac edition on my Mac,
following the instructions
from [the VCVRack README](https://github.com/vcvrack/rack).

[Travis CI](https://travis-ci.org)
can build the Mac version.
I haven't configured that,
because I don't need it.

## Building on Windows

I build the Windows edition
using [AppVeyor.com](https://appveyor.com).

- The `.appveyor.yml` file configures the build

From [the VCVRack README](https://github.com/vcvrack/rack):

> Install MSYS2 and launch the mingw64 shell (not the default msys2 shell).

The AppVeyor environment includes MSYS2, so we don't need to install it.

`.appveyor.yml` configures the shell by setting MSYSTEM=MINGW64.
As far as I can tell,
bash will react to this
by configuring itself to use the mingw64 environment.

From [the VCVRack README](https://github.com/vcvrack/rack):

> Install build dependencies with the `pacman` package manger.
>
> `pacman -S git make tar zip unzip mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake`

AppVeyor's MSYS2 installation includes these packages, so we don't need to install them:
- `git`
- `make`
- `tar`
- `mingw-w64-x86_64-gcc`

So we need only install these packages:
- `zip`
- `unzip`
- `mingw-w64-x86_64-cmake`
