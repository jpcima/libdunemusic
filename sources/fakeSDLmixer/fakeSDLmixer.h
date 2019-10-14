#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_audio.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIX_DEFAULT_FREQUENCY 22050
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
#define MIX_DEFAULT_FORMAT AUDIO_S16LSB
#else
#define MIX_DEFAULT_FORMAT AUDIO_S16MSB
#endif
#define MIX_DEFAULT_CHANNELS 2
#define MIX_MAX_VOLUME SDL_MIX_MAXVOLUME

typedef struct Mix_Chunk {
    int allocated;
    Uint8 *abuf;
    Uint32 alen;
    Uint8 volume;
} Mix_Chunk;

int SDLCALL Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize);
void SDLCALL Mix_CloseAudio(void);
int SDLCALL Mix_QuerySpec(int *frequency, Uint16 *format, int *channels);
void SDLCALL Mix_HookMusic(void (SDLCALL *mix_func)(void *udata, Uint8 *stream, int len), void *arg);
int SDLCALL Mix_VolumeMusic(int volume);
void SDLCALL Mix_FreeChunk(Mix_Chunk *chunk);

/**/
void SDLCALL FakeMix_ProcessFrames(Sint16 *frames, unsigned count);

#ifdef __cplusplus
}
#endif
