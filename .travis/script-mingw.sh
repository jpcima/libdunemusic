#!/bin/bash

# ---------------------------------------------------------------------------------------------------------------------
# stop on error

set -e

# ---------------------------------------------------------------------------------------------------------------------
# print shell commands

set -x

# ---------------------------------------------------------------------------------------------------------------------
# set variables

test ! -z "$_MINGW"
test ! -z "$_HOST"

# ---------------------------------------------------------------------------------------------------------------------
# setup

export AR="$_HOST"-ar
export RANLIB="$_HOST"-ranlib
export CC="$_HOST"-gcc
export CXX="$_HOST"-g++
export STRIP="$_HOST"-strip
export WINDRES="$_HOST"-windres
export PKG_CONFIG="$_HOST"-pkg-config
export CMAKE="$_HOST"-cmake

export PKG_CONFIG="$PKG_CONFIG --static"

export PATH=/opt/"$_MINGW"/"$_HOST"/bin:/opt/"$_MINGW"/bin:"$PATH"

export CFLAGS="-I/opt/$_MINGW/include"
export CXXFLAGS="-I/opt/$_MINGW/include"
export LDFLAGS="-L/opt/$_MINGW/lib"

# ---------------------------------------------------------------------------------------------------------------------

cat >>/etc/pacman.conf <<EOF
[multilib]
Include = /etc/pacman.d/mirrorlist

[mingw-w64]
SigLevel = Optional TrustAll
Server = https://github.com/jpcima/arch-mingw-w64/releases/download/repo.\$arch/
EOF

pacman -Sqy --noconfirm
pacman -Sq --noconfirm base-devel cmake mingw-w64-gcc mingw-w64-pkg-config mingw-w64-sdl2

# ---------------------------------------------------------------------------------------------------------------------
# build the library

"$CMAKE" -B build -DCMAKE_BUILD_TYPE=Release -DUSE_STDCALL=ON -DDUNEMUSIC_BUILD_SDL2=ON .
"$CMAKE" --build build
