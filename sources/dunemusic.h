#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DUNEMUSIC_EXPORT
#   if defined(_WIN32) && defined(DUNEMUSIC_BUILD)
#       define DUNEMUSIC_EXPORT __declspec(dllexport)
#   elif defined(_WIN32)
#       define DUNEMUSIC_EXPORT __declspec(dllimport)
#   elif defined(__GNUC__)
#       define DUNEMUSIC_EXPORT __attribute__((visibility("default")))
#   endif
#endif

typedef enum DuneMusicOplEmu {
    kOplEmuWoody,
    kOplEmuDosbox,
    kOplEmuNuked,
} DuneMusicOplEmu;

DUNEMUSIC_EXPORT void DuneMusic_Init(int sampleRate, const char *dataDirectory, DuneMusicOplEmu oplEmu);
DUNEMUSIC_EXPORT void DuneMusic_Quit();

DUNEMUSIC_EXPORT void DuneMusic_GetSamples(int16_t *buf, unsigned count);

DUNEMUSIC_EXPORT void DuneMusic_InsertMemoryFile(const char *filename, const void *data, size_t length);

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

DUNEMUSIC_EXPORT void DuneMusic_ChangeMusic(DuneMusicType musicType);
DUNEMUSIC_EXPORT void DuneMusic_ChangeMusicEx(DuneMusicType musicType, const char *filename, int musicNum);

DUNEMUSIC_EXPORT int16_t *DuneMusic_SynthesizeAudio(const char *filename, int musicNum, int volume, size_t *numFramesReturned);
DUNEMUSIC_EXPORT void DuneMusic_FreeAudio(int16_t *audioBuffer);

DUNEMUSIC_EXPORT int DuneMusic_IsMusicEnabled();
DUNEMUSIC_EXPORT void DuneMusic_SetMusicEnabled(int enabled);
DUNEMUSIC_EXPORT int DuneMusic_IsMusicPlaying();
DUNEMUSIC_EXPORT void DuneMusic_SetMusicVolume(int volume);

DUNEMUSIC_EXPORT unsigned DuneMusic_GetKnownMusicCount();
DUNEMUSIC_EXPORT void DuneMusic_SetKnownMusicIndex(unsigned index);

#ifdef __cplusplus
}
#endif
