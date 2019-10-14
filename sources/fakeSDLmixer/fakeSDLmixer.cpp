#include "fakeSDLmixer.h"
#include <mutex>
#include <memory>
#include <cassert>

///
struct MixerData {
    uint init;
    uint sampleRate;
    uint channels;
    void (SDLCALL *hookFunc)(void *udata, Uint8 *stream, int len);
    void *hookData;
};

static std::mutex sMutex;
static std::unique_ptr<MixerData> sMixerData;

///
int SDLCALL Mix_OpenAudio(int frequency, Uint16 format, int channels, int chunksize)
{
    std::lock_guard<std::mutex> lock(sMutex);

    MixerData *mixerData = sMixerData.get();
    if (mixerData) {
        mixerData->init += 1;
        return 0;
    }

    if (frequency <= 0 || format != MIX_DEFAULT_FORMAT || channels <= 0)
        return -1;

    mixerData = new MixerData;
    sMixerData.reset(mixerData);

    mixerData->init = 1;
    mixerData->sampleRate = frequency;
    mixerData->channels = channels;
    mixerData->hookFunc = nullptr;
    mixerData->hookData = nullptr;

    return 0;
}

void SDLCALL Mix_CloseAudio(void)
{
    std::lock_guard<std::mutex> lock(sMutex);

    MixerData *mixerData = sMixerData.get();
    if (mixerData && --mixerData->init == 0)
        sMixerData.reset();
}

int SDLCALL Mix_QuerySpec(int *frequency, Uint16 *format, int *channels)
{
    MixerData *mixerData = sMixerData.get();
    assert(mixerData);

    if (frequency)
        *frequency = mixerData->sampleRate;
    if (format)
        *format = MIX_DEFAULT_FORMAT;
    if (channels)
        *channels = mixerData->channels;

    return 0;
}

void SDLCALL Mix_HookMusic(void (SDLCALL *mix_func)(void *udata, Uint8 *stream, int len), void *arg)
{
    std::lock_guard<std::mutex> lock(sMutex);

    MixerData *mixerData = sMixerData.get();
    assert(mixerData);

    mixerData->hookFunc = mix_func;
    mixerData->hookData = arg;
}

int SDLCALL Mix_VolumeMusic(int volume)
{
    /* TODO volume */
    return 0;
}

void SDLCALL Mix_FreeChunk(Mix_Chunk *chunk)
{
    if (chunk) {
        if (chunk->allocated)
            SDL_free(chunk->abuf);
        SDL_free(chunk);
    }
}

void SDLCALL FakeMix_ProcessFrames(Sint16 *frames, unsigned count)
{
    std::lock_guard<std::mutex> lock(sMutex);

    MixerData *mixerData = sMixerData.get();

    if (mixerData->hookFunc)
        mixerData->hookFunc(mixerData->hookData, (Uint8 *)frames, count * sizeof(Sint16) * mixerData->channels);
    /* TODO volume */
}
