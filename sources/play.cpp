#include "dunemusic.h"
#include <SDL2/SDL.h>
#include <random>
#include <cstring>

static constexpr unsigned kSampleRate = 44100;
static void SDLCALL ProcessAudio(void *userdata, Uint8 *stream, int len);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Please pass the Dune Legacy directory as argument.\n");
        return 1;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "SDL_Init failed.\n");
        return 1;
    }

    DuneMusicOplEmu oplEmu = kOplEmuNuked;
    if (argc >= 3)
        oplEmu = (DuneMusicOplEmu)atoi(argv[2]);

    DuneMusic_Init(kSampleRate, argv[1], oplEmu);

    SDL_AudioSpec spec = {};
    spec.freq = kSampleRate;
    spec.format = AUDIO_S16SYS;
    spec.channels = 2;
    spec.samples = 1024;
    spec.callback = &ProcessAudio;

    if (SDL_OpenAudio(&spec, nullptr) != 0) {
        fprintf(stderr, "SDL_OpenAudio failed.\n");
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Player", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 300, 300, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed.\n");
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    DuneMusic_SetMusicEnabled(1);
    SDL_PauseAudio(0);

    std::default_random_engine prng{std::random_device{}()};
    unsigned knownMusicCount = DuneMusic_GetKnownMusicCount();

    SDL_Event event;
    bool quit = false;
    while (!quit && SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            default:
                if (knownMusicCount > 0) {
                    DuneMusic_SetKnownMusicIndex(
                        std::uniform_int_distribution<unsigned>{0, knownMusicCount - 1}(prng));
                }
                break;
            }
            break;
        case SDL_QUIT:
            quit = true;
            break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_CloseAudio();
    DuneMusic_Quit();
    SDL_Quit();

    return 0;
}

static void SDLCALL ProcessAudio(void *userdata, Uint8 *stream, int len)
{
    DuneMusic_GetSamples((Sint16 *)stream, len / (2 * sizeof(Uint16)));
}
