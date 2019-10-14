#include "dunemusic.h"
#include "music_list.h"
#include "FileClasses/FileManager.h"
#include "FileClasses/music/ADLPlayer.h"
#include "FileClasses/adl/sound_adlib.h"
#include <fakeSDLmixer.h>

#if defined(_WIN32)
#define DUNEMUSIC_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
#define DUNEMUSIC_EXPORT __attribute__((visibility("default")))
#endif

std::unique_ptr<FileManager> pFileManager;

static std::unique_ptr<ADLPlayer> sPlayer;

std::string duneLegacyDataDir;

DUNEMUSIC_EXPORT
void DuneMusic_Init(int sampleRate, const char *dataDirectory, DuneMusicOplEmu oplEmu)
{
    Mix_OpenAudio(sampleRate, MIX_DEFAULT_FORMAT, 2, 1024);
    duneLegacyDataDir.assign(dataDirectory);
    pFileManager.reset(new FileManager);
    SoundAdlibPC::s_oplEmu = oplEmu;
    sPlayer.reset(new ADLPlayer);
}

DUNEMUSIC_EXPORT
void DuneMusic_Quit()
{
    sPlayer.reset();
    pFileManager.reset();
    Mix_CloseAudio();
}

DUNEMUSIC_EXPORT
void DuneMusic_GetSamples(int16_t *buf, unsigned count)
{
    FakeMix_ProcessFrames(buf, count);
}

DUNEMUSIC_EXPORT
void DuneMusic_ChangeMusic(DuneMusicType musicType)
{
    sPlayer->changeMusic((MUSICTYPE)musicType);
}

DUNEMUSIC_EXPORT
void DuneMusic_ChangeMusicEx(DuneMusicType musicType, const char *filename, int musicNum)
{
    sPlayer->changeMusicEx((MUSICTYPE)musicType, filename, musicNum);
}

DUNEMUSIC_EXPORT
int DuneMusic_IsMusicEnabled()
{
    return sPlayer->isMusicOn();
}

DUNEMUSIC_EXPORT
void DuneMusic_SetMusicEnabled(int enabled)
{
    sPlayer->setMusic(enabled);
}

DUNEMUSIC_EXPORT
int DuneMusic_IsMusicPlaying()
{
    return sPlayer->isMusicPlaying();
}

DUNEMUSIC_EXPORT
void DuneMusic_SetMusicVolume(int volume)
{
    sPlayer->setMusicVolume(volume);
}

DUNEMUSIC_EXPORT
unsigned DuneMusic_GetKnownMusicCount()
{
    return numMusicTracks;
}

DUNEMUSIC_EXPORT
void DuneMusic_SetKnownMusicIndex(unsigned index)
{
    if (index >= numMusicTracks)
        return;

    const MusicDefinition *md = &musicTracks[index];
    sPlayer->changeMusicEx(md->type, md->filename, md->musicNum);
}
