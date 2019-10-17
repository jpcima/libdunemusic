﻿using System;
using System.Runtime.InteropServices;

namespace DuneMusic
{
    public class DuneMusic
    {
        public enum DuneMusicOplEmu
        {
            kOplEmuWoody,
            kOplEmuDosbox,
            kOplEmuNuked,
        };

        public static void Init(int sampleRate, string dataDirectory, DuneMusicOplEmu oplEmu)
        {
            DuneMusic_Init(sampleRate, UTF8_ToNative(dataDirectory), oplEmu);
        }

        public static void Quit()
        {
            DuneMusic_Quit();
        }

        public static void GetSamples(Int16[] buf)
        {
            DuneMusic_GetSamples(buf, (uint)(buf.Length / 2));
        }

        public static void InsertMemoryFile(string filename, byte[] data)
        {
            DuneMusic_InsertMemoryFile(UTF8_ToNative(filename), data, (UIntPtr)data.Length);
        }

        public enum DuneMusicType {
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
        };

        public static void ChangeMusic(DuneMusicType musicType)
        {
            DuneMusic_ChangeMusic(musicType);
        }

        public static void ChangeMusicEx(DuneMusicType musicType, string filename, int musicNum)
        {
            DuneMusic_ChangeMusicEx(musicType, UTF8_ToNative(filename), musicNum);
        }

        public static Int16[] SynthesizeAudio(string filename, int musicNum, int volume)
        {
            UIntPtr numFrames;
            Int16[] cBuffer = DuneMusic_SynthesizeAudio(UTF8_ToNative(filename), musicNum, volume, out numFrames);
            Int16[] buffer = cBuffer.Clone() as Int16[];
            DuneMusic_FreeAudio(cBuffer);
            return buffer;
        }

        public static bool IsMusicEnabled()
        {
            return DuneMusic_IsMusicEnabled() != 0;
        }

        public static void SetMusicEnabled(bool enabled)
        {
            DuneMusic_SetMusicEnabled(enabled ? 1 : 0);
        }

        public static bool IsMusicPlaying()
        {
            return DuneMusic_IsMusicPlaying() != 0;
        }

        public static void SetMusicVolume(int volume)
        {
            DuneMusic_SetMusicVolume(volume);
        }

        public static uint GetKnownMusicCount()
        {
            return DuneMusic_GetKnownMusicCount();
        }

        public static void SetKnownMusicIndex(uint index)
        {
            DuneMusic_SetKnownMusicIndex(index);
        }

        private const string nativeLibName = "dunemusic";

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_Init(int sampleRate, byte[] dataDirectory, DuneMusicOplEmu oplEmu);
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_Quit();

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_GetSamples(Int16[] buf, uint numFrames);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_InsertMemoryFile(byte[] filename, byte[] data, UIntPtr length);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_ChangeMusic(DuneMusicType musicType);
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_ChangeMusicEx(DuneMusicType musicType, byte[] filename, int musicNum);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.LPArray, ArraySubType = UnmanagedType.I2, SizeParamIndex = 3)]
        private static extern Int16[] DuneMusic_SynthesizeAudio(byte[] filename, int musicNum, int volume, out UIntPtr numFramesReturned);
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_FreeAudio(Int16[] audioBuffer);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int DuneMusic_IsMusicEnabled();

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_SetMusicEnabled(int enabled);
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int DuneMusic_IsMusicPlaying();
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_SetMusicVolume(int volume);

        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern uint DuneMusic_GetKnownMusicCount();
        [DllImport(nativeLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void DuneMusic_SetKnownMusicIndex(uint index);

        private static byte[] UTF8_ToNative(string s)
        {
            if (s == null)
                return null;
            return System.Text.Encoding.UTF8.GetBytes(s + '\0');
        }
    }
}
