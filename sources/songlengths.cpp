#include "dunemusic.h"
#include "music_list.h"
#include <cmath>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Pease indicate the <data-directory>.\n");
        return 1;
    }

    const int sampleRate = 44100;

    for (size_t i = 0; i < numMusicTracks; ++i) {
        const MusicDefinition &md = musicTracks[i];

        DuneMusic_Init(sampleRate, argv[1], kOplEmuDosbox);
        DuneMusic_SetMusicEnabled(1);
        DuneMusic_SetKnownMusicIndex(i);

        size_t framesPerBuffer = 256;
        double maxDuration = 60 * 30; // 30 minutes
        size_t maxFrames = std::ceil(maxDuration * sampleRate);
        size_t genFrames = 0;

        int16_t buffer[2 * framesPerBuffer];
        while (DuneMusic_IsMusicPlaying() && genFrames < maxFrames) {
            DuneMusic_GetSamples((uint8_t *)buffer, sizeof(buffer));
            genFrames += framesPerBuffer;
        }

        if (genFrames >= maxFrames) {
            printf("%s %u \"%s\": infinite\n", md.filename, md.musicNum, md.title);
        }
        else {
            printf("%s %u \"%s\": %f seconds\n", md.filename, md.musicNum, md.title, (double)genFrames / sampleRate);
        }
        fflush(stdout);

        DuneMusic_Quit();
    }

    return 0;
}
