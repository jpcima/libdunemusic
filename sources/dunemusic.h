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

#ifdef __WIN32
#   if defined(DUNEMUSIC_CDECL) && !defined(DUNEMUSIC_STDCALL)
#       define DMCALL __cdecl
#       define DMCALLCC
#   elif defined(DUNEMUSIC_STDCALL) && !defined(DUNEMUSIC_CDECL)
#       define DMCALL __stdcall
#       define DMCALLCC __stdcall
#   else
#       error You must define either DUNEMUSIC_CDECL or DUNEMUSIC_STDCALL.
#   endif
#else
#   define DMCALL
#   define DMCALLCC
#endif

typedef enum DuneMusicOplEmu {
    kOplEmuWoody,
    kOplEmuDosbox,
    kOplEmuNuked,
} DuneMusicOplEmu;

DUNEMUSIC_EXPORT void DMCALL DuneMusic_Init(int sampleRate, const char *dataDirectory, DuneMusicOplEmu oplEmu);
DUNEMUSIC_EXPORT void DMCALL DuneMusic_Quit();

DUNEMUSIC_EXPORT void DMCALL DuneMusic_GetSamples(uint8_t *buf, unsigned count);

DUNEMUSIC_EXPORT void DMCALL DuneMusic_InsertMemoryFile(const char *filename, const void *data, size_t length);

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

DUNEMUSIC_EXPORT void DMCALL DuneMusic_ChangeMusic(DuneMusicType musicType);
DUNEMUSIC_EXPORT void DMCALL DuneMusic_ChangeMusicEx(DuneMusicType musicType, const char *filename, int musicNum);

/**
   @param volume use `-1` for default, which is identical to specifying `64`
   @param soundBuf pointer to an output array of minimum capacity `maxBytes`
   @param maxBytes maximum number of bytes which can be received in `soundBuf`
   @return actual number N of bytes synthesized, `soundBuf` contains `N` elements.
 */
DUNEMUSIC_EXPORT size_t DMCALL DuneMusic_SynthesizeAudio(const char *filename, int musicNum, int volume, uint8_t *soundBuf, size_t maxBytes);

DUNEMUSIC_EXPORT int DMCALL DuneMusic_IsMusicEnabled();
DUNEMUSIC_EXPORT void DMCALL DuneMusic_SetMusicEnabled(int enabled);
DUNEMUSIC_EXPORT int DMCALL DuneMusic_IsMusicPlaying();
DUNEMUSIC_EXPORT void DMCALL DuneMusic_SetMusicVolume(int volume);

DUNEMUSIC_EXPORT unsigned DMCALL DuneMusic_GetKnownMusicCount();
DUNEMUSIC_EXPORT void DMCALL DuneMusic_SetKnownMusicIndex(unsigned index);

#ifdef __cplusplus
}
#endif
