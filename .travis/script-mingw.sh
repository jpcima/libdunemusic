#!/bin/bash

# ---------------------------------------------------------------------------------------------------------------------
# stop on error

set -e

# ---------------------------------------------------------------------------------------------------------------------
# print shell commands

set -x

# ---------------------------------------------------------------------------------------------------------------------
# set variables

test ! -z "$_DM_MINGW"
test ! -z "$_DM_HOST"

# ---------------------------------------------------------------------------------------------------------------------

cat >>/etc/pacman.conf <<EOF
[multilib]
Include = /etc/pacman.d/mirrorlist

[mingw-w64]
SigLevel = Optional TrustAll
Server = https://github.com/jpcima/arch-mingw-w64/releases/download/repo.\$arch/
EOF

pacman -Sqy --noconfirm
pacman -Sq --noconfirm base-devel git mercurial mingw-w64-cmake mingw-w64-gcc mingw-w64-pkg-config mingw-w64-sdl2 mingw-w64-win-iconv

# ---------------------------------------------------------------------------------------------------------------------
# build the library

mkdir build
pushd build
"$_DM_HOST"-cmake \
           -DCMAKE_BUILD_TYPE=Release \
           -DUSE_STDCALL="$_DM_USE_STDCALL" \
           -DDUNEMUSIC_SDL2_LIBNAME="SDL2-static" \
           -DDUNEMUSIC_BUILD_FORCE_STATIC=ON \
           ..
make
popd
