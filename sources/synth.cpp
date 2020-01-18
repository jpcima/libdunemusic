#include "dunemusic.h"
#include <sndfile.h>
#include <getopt.h>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cmath>

static constexpr unsigned kSampleRate = 44100;

int main(int argc, char *argv[])
{
    const char *outputFile = "dunemusic.wav";
    double maxDuration = 10.0;

    for (int c; (c = getopt(argc, argv, "o:")) != -1;) {
        switch (c) {
        case 'o':
            outputFile = optarg;
            break;
        default:
            return 1;
        }
    }

    if (argc - optind != 3) {
        fprintf(stderr, "Please indicate <data-directory>, <music-file> and <song-number>.\n");
        return 1;
    }

    const char *directory = argv[optind];
    const char *musicFilename = argv[optind + 1];
    int musicNum = atoi(argv[optind + 2]);

    DuneMusicOplEmu oplEmu = kOplEmuDosbox;
    DuneMusic_Init(kSampleRate, directory, oplEmu);

    fprintf(stderr, "* Generate song %d of \"%s\"\n", musicNum, musicFilename);
    fprintf(stderr, "* Write to \"%s\"\n", outputFile);

    unsigned maxFrames = ceil(maxDuration * kSampleRate);
    std::vector<int16_t> soundBuf(2 * maxFrames);

    size_t numFrames = DuneMusic_SynthesizeAudio(
        musicFilename, musicNum, -1, (uint8_t *)soundBuf.data(), maxFrames * (2 * sizeof(int16_t)));

    if (numFrames <= 0) {
        fprintf(stderr, "Failed to synthesize song.\n");
        return 1;
    }

    fprintf(stderr, "* Buffer capacity: %lu frames\n", (unsigned long)maxFrames);
    fprintf(stderr, "* Actual length: %lu frames\n", (unsigned long)numFrames);

    soundBuf.resize(2 * numFrames);

    SF_INFO sfinfo = {};
    sfinfo.frames = numFrames;
    sfinfo.samplerate = kSampleRate;
    sfinfo.channels = 2;
    sfinfo.format = SF_FORMAT_WAV|SF_FORMAT_PCM_16;

    SNDFILE *sf = sf_open(outputFile, SFM_WRITE, &sfinfo);
    if (!sf) {
        fprintf(stderr, "Failed to open the sound file for writing.\n");
        return 1;
    }

    sf_writef_short(sf, soundBuf.data(), numFrames);
    sf_close(sf);

    return 0;
}
