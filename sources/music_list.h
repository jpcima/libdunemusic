#pragma once
#include "FileClasses/music/MusicPlayer.h"

struct MusicDefinition {
    MUSICTYPE type;
    const char *filename;
    int musicNum;
    const char *title;
};

static const MusicDefinition musicTracks[] = {
    {MUSIC_ATTACK, "DUNE10.ADL", 7, "Attack 1"},
    {MUSIC_ATTACK, "DUNE11.ADL", 7, "Attack 2"},
    {MUSIC_ATTACK, "DUNE12.ADL", 7, "Attack 3"},
    {MUSIC_ATTACK, "DUNE13.ADL", 7, "Attack 4"},
    {MUSIC_ATTACK, "DUNE14.ADL", 7, "Attack 5"},
    {MUSIC_ATTACK, "DUNE15.ADL", 7, "Attack 6"},

    {MUSIC_PEACE, "DUNE1.ADL" , 6, "Peace 1"},
    {MUSIC_PEACE, "DUNE2.ADL" , 6, "Peace 2"},
    {MUSIC_PEACE, "DUNE3.ADL" , 6, "Peace 3"},
    {MUSIC_PEACE, "DUNE4.ADL" , 6, "Peace 4"},
    {MUSIC_PEACE, "DUNE5.ADL" , 6, "Peace 5"},
    {MUSIC_PEACE, "DUNE6.ADL" , 6, "Peace 6"},
    {MUSIC_PEACE, "DUNE9.ADL" , 4, "Peace 7"},
    {MUSIC_PEACE, "DUNE9.ADL" , 5, "Peace 8"},
    {MUSIC_PEACE, "DUNE18.ADL", 6, "Peace 9"},

    {MUSIC_INTRO, "DUNE0.ADL", 2, "Intro"},
    {MUSIC_MENU, "DUNE7.ADL", 6, "Menu"},
    {MUSIC_BRIEFING_H, "DUNE7.ADL", 2, "Briefing H"},
    {MUSIC_BRIEFING_A, "DUNE7.ADL", 3, "Briefing A"},
    {MUSIC_BRIEFING_O, "DUNE7.ADL", 4, "Briefing O"},
    {MUSIC_WIN_H, "DUNE8.ADL", 3, "Win H"},
    {MUSIC_WIN_A, "DUNE8.ADL", 2, "Win A"},
    {MUSIC_WIN_O, "DUNE17.ADL", 4, "Win O"},
    {MUSIC_LOSE_H, "DUNE1.ADL", 4, "Lose H"},
    {MUSIC_LOSE_A, "DUNE1.ADL", 5, "Lose A"},
    {MUSIC_LOSE_O, "DUNE1.ADL", 3, "Lose O"},
    {MUSIC_GAMESTAT, "DUNE20.ADL", 2, "Game stat"},
    {MUSIC_MAPCHOICE, "DUNE16.ADL", 7, "Map choice"},
    {MUSIC_MEANWHILE, "DUNE16.ADL", 8, "Meanwhile"},
    {MUSIC_FINALE_H, "DUNE19.ADL", 4, "Finale H"},
    {MUSIC_FINALE_A, "DUNE19.ADL", 2, "Finale A"},
    {MUSIC_FINALE_O, "DUNE19.ADL", 3, "Finale O"},
};

static constexpr size_t numMusicTracks = sizeof(musicTracks) / sizeof(musicTracks[0]);
