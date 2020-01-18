#pragma once
#include "portable_endian.h"

#include <stdio.h>
#include <stdint.h>

typedef uint8_t Uint8;
typedef uint16_t Uint16;
typedef uint32_t Uint32;
typedef uint64_t Uint64;

typedef int8_t Sint8;
typedef int16_t Sint16;
typedef int32_t Sint32;
typedef int64_t Sint64;

#define SDL_malloc malloc
#define SDL_calloc calloc
#define SDL_realloc realloc
#define SDL_free free

#define SDLCALL

#define SDL_Log(fmt, ...) fprintf(stderr, fmt "\n", ## __VA_ARGS__)

#define SDL_LIL_ENDIAN  __LITTLE_ENDIAN
#define SDL_BIG_ENDIAN  __BIG_ENDIAN
#define SDL_BYTEORDER __BYTE_ORDER

#define SDL_SwapLE16 le16toh
#define SDL_SwapLE32 le32toh
#define SDL_SwapLE64 le64toh
#define SDL_SwapBE16 be16toh
#define SDL_SwapBE32 be32toh
#define SDL_SwapBE64 be64toh
