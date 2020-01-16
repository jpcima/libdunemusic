#include "dunemusic.h"
#include "music_list.h"
#include "FileClasses/FileManager.h"
#include "FileClasses/music/ADLPlayer.h"
#include "FileClasses/adl/sound_adlib.h"
#include <fakeSDLmixer.h>

std::unique_ptr<FileManager> pFileManager;

static std::unique_ptr<ADLPlayer> sPlayer;

std::string duneLegacyDataDir;

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_Init(int sampleRate, const char *dataDirectory, DuneMusicOplEmu oplEmu)
{
    Mix_OpenAudio(sampleRate, MIX_DEFAULT_FORMAT, 2, 1024);

    if (dataDirectory && dataDirectory[0])
        duneLegacyDataDir.assign(dataDirectory);
    else
        duneLegacyDataDir.assign(".");

    pFileManager.reset(new FileManager);
    SoundAdlibPC::s_oplEmu = oplEmu;
    sPlayer.reset(new ADLPlayer);
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_Quit()
{
    sPlayer.reset();
    pFileManager.reset();
    Mix_CloseAudio();
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_InsertMemoryFile(const char *filename, const void *data, size_t length)
{
    pFileManager->insertMemoryFile(filename, data, length);
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_GetSamples(int16_t *buf, unsigned count)
{
    FakeMix_ProcessFrames(buf, count);
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_ChangeMusic(DuneMusicType musicType)
{
    sPlayer->changeMusic((MUSICTYPE)musicType);
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_ChangeMusicEx(DuneMusicType musicType, const char *filename, int musicNum)
{
    sPlayer->changeMusicEx((MUSICTYPE)musicType, filename, musicNum);
}

DUNEMUSIC_EXPORT
size_t DMCALL DuneMusic_SynthesizeAudio(const char *filename, int musicNum, int volume, int16_t *soundBuf, size_t maxFrames)
{
    sdl2::RWops_ptr rwop = pFileManager->openFile(filename);
    if (!rwop)
        return 0;

    std::unique_ptr<SoundAdlibPC> adlib{new SoundAdlibPC(rwop.get())};

    if (volume < 0)
        volume = 64;

    adlib->setVolume(volume);

    return adlib->getSubsongWithMaxLength(musicNum, soundBuf, maxFrames);
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_FreeAudio(int16_t *audioBuffer)
{
    SDL_free(audioBuffer);
}

DUNEMUSIC_EXPORT
int DMCALLCC DuneMusic_IsMusicEnabled()
{
    return sPlayer->isMusicOn();
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_SetMusicEnabled(int enabled)
{
    sPlayer->setMusic(enabled);
}

DUNEMUSIC_EXPORT
int DMCALLCC DuneMusic_IsMusicPlaying()
{
    return sPlayer->isMusicPlaying();
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_SetMusicVolume(int volume)
{
    sPlayer->setMusicVolume(volume);
}

DUNEMUSIC_EXPORT
unsigned DMCALLCC DuneMusic_GetKnownMusicCount()
{
    return numMusicTracks;
}

DUNEMUSIC_EXPORT
void DMCALLCC DuneMusic_SetKnownMusicIndex(unsigned index)
{
    if (index >= numMusicTracks)
        return;

    const MusicDefinition *md = &musicTracks[index];
    sPlayer->changeMusicEx(md->type, md->filename, md->musicNum);
}
