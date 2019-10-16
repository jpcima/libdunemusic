#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum DuneMusicOplEmu {
    kOplEmuWoody,
    kOplEmuDosbox,
    kOplEmuNuked,
} DuneMusicOplEmu;

void DuneMusic_Init(int sampleRate, const char *dataDirectory, DuneMusicOplEmu oplEmu);
void DuneMusic_Quit();

void DuneMusic_GetSamples(int16_t *buf, unsigned count);

void DuneMusic_InsertMemoryFile(const char *filename, const void *data, size_t length);

typedef enum DuneMusicType {
    kMusicAttack = 0,   /*!< Played when at least one of player's units was hit. */
    kMusicPeace,        /*!< Played most of the time when the enemy is not attacking. */
    kMusicIntro,        /*!< Background music for intro. */
    kMusicMenu,         /*!< The menu background music. */
    kMusicBriefingH,    /*!< Harkonnen briefing background music. */
    kMusicBriefingA,    /*!< Atreides briefing background music. */
    kMusicBriefingO,    /*!< Ordos briefing background music. */
    kMusicWinH,         /*!< Harkonnen victory screen background music.. */
    kMusicWinA,         /*!< Atreides victory screen background music.. */
    kMusicWinO,         /*!< Ordos victory screen background music.. */
    kMusicLoseH,        /*!< Harkonnen failure screen background music.. */
    kMusicLoseA,        /*!< Atreides failure screen background music.. */
    kMusicLoseO,        /*!< Ordos failure screen background music.. */
    kMusicGameStat,     /*!< Game statistics background music. */
    kMusicMapChoice,    /*!< The map choice background music. */
    kMusicMeanwhile,    /*!< Meanwhile cutscene background music. */
    kMusicFinaleH,      /*!< Harkonnen finale cutscene background music. */
    kMusicFinaleA,      /*!< Atreides finale cutscene background music. */
    kMusicFinaleO,      /*!< Ordos finale cutscene background music. */

    kMusicNumTypes,

    kMusicRandom = 0xFF /*!< Random music (attack music or peace music) */
} DuneMusicType;

void DuneMusic_ChangeMusic(DuneMusicType musicType);
void DuneMusic_ChangeMusicEx(DuneMusicType musicType, const char *filename, int musicNum);

int16_t *DuneMusic_SynthesizeAudio(const char *filename, int musicNum, int volume, size_t *numFramesReturned);
void DuneMusic_FreeAudio(int16_t *audioBuffer);

int DuneMusic_IsMusicEnabled();
void DuneMusic_SetMusicEnabled(int enabled);
int DuneMusic_IsMusicPlaying();
void DuneMusic_SetMusicVolume(int volume);

unsigned DuneMusic_GetKnownMusicCount();
void DuneMusic_SetKnownMusicIndex(unsigned index);

#ifdef __cplusplus
}
#endif
