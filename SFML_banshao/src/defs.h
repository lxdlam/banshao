#pragma once
#include <string>
#include <vector>

using std::string;
namespace game::defs
{
	namespace general
	{
		// Game 
		const string name = "asdf";
		const string subname = "alpha";
		const unsigned versionMajor = 0;
		const unsigned versionMinor = 1;

		// Mode
		enum class eMode {
			UNKNOWN = -1,

			EXIT = 0,
			TITLE = 1,
			SONG_SELECT,
			THEME_SELECT,
			KEY_CONFIG,

			PLAY5,
			PLAY5_2,
			PLAY7,
			PLAY7_2,
			PLAY9,
			PLAY10,
			PLAY14,

			RESULT,
			COURSE_RESULT,

			TMPL,
			TEST
		};
	}

	namespace key
	{
		// Keys
		enum gamepadKeys
		{
			S1L = 0,
			S1R,
			K11,
			K12,
			K13,
			K14,
			K15,
			K16,
			K17,
			K18,
			K19,
			K1START,
			K1SELECT,
			K1SPDUP,
			K1SPDDN,

			S2L,
			S2R,
			K21,
			K22,
			K23,
			K24,
			K25,
			K26,
			K27,
			K28,
			K29,
			K2START,
			K2SELECT,
			K2SPDUP,
			K2SPDDN,
			GAMEPAD_KEY_COUNT
		};
		enum functionalKeys
		{
			ESC = 0,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			UP,
			DOWN,
			LEFT,
			RIGHT,
			RETURN,
			BACKSPACE,
			FUNC_KEY_COUNT
		};
		static constexpr unsigned long mask(gamepadKeys k) { return 1 << k; }
		static constexpr unsigned long mask(functionalKeys k) { return 1 << k; }
		const unsigned KEYSLOTS = 10;
	}

	// Config keys
	namespace config
	{
		// system
		const string sys_basespd = "Basespeed";
		// video
		const string vid_FullHD = "FullHD";
		const string vid_fullscreen = "FullScreen";
		const string vid_borderless = "Borderless";
		const string vid_maxfps = "MaxFPS";
		const string vid_vsync = "VSync";
		// audio
		enum audioMode
		{
			AutoDetect = 0,
			Nosound,
			Unknown,
			GENERAL_COUNT,

			// Windows
			WinMM = 10,
			DSound,
			WASAPI,
			ASIO,
			Atmos,
			WINDOWS_COUNT,

			// Linux
			PulseAudio = 20,
			ALSA,
			LINUX_COUNT,

			// Mac
			CoreAudio = 30,
			MAC_COUNT
		};
		const string aud_mode = "OutputMode";
		const string aud_bufLen = "BufferLength";
		const string aud_bufCount = "BufferCount";
		// play
		const string pl_hs1 = "HiSpeed1P";
		const string pl_hs2 = "HiSpeed2P";
		const string pl_sud1 = "Sud+1P";
		const string pl_sud2 = "Sud+2P";
		const string pl_hid1 = "Hid+1P";
		const string pl_hid2 = "Hid+2P";
		const string pl_lift1 = "Lift+1P";
		const string pl_lift2 = "Lift+2P";
		const string pl_gr1 = "Green1P";
		const string pl_gr2 = "Green2P";
		const string pl_mod1 = "Mode1P";
		const string pl_mod2 = "Mode2P";
		// key
		const string k_5keys = "5keys";
		const string k_7keys = "7keys";
		const string k_9keys = "9keys";
	}

	// mod
	namespace mod
	{
		enum playMod
		{
			// random
			RANDOM = 0,
			MIRROR,
			SRAN,
			HRAN,			// Scatter
			ALLSCR,			// Converge

			// gauge
			HARD = 6,
			EASY,
			DEATH,
			PATTACK,
			GATTACK,

			// assist
			AUTO67 = 12,	// 5keys
			AUTOSCR,
			LEGACY,			// LN -> Note
			NOMINES,

			// hs fix
			MAXBPM = 18,
			MINBPM,
			AVERAGE,
			CONSTANT,

			// effect
			SUDDEN = 24,
			HIDDEN,
			LIFT,

		};
		static constexpr unsigned long mask(playMod k) { return 1 << k; }
	}

	// bms related
	namespace bms
	{
		const unsigned BGMCHANNELS = 32;
		const unsigned MAXSAMPLEIDX = 36 * 36;
		const unsigned MAXMEASUREIDX = 999;
		enum bmsMode {
			MODE_5KEYS,
			MODE_7KEYS,
			MODE_9KEYS,
			MODE_10KEYS,
			MODE_14KEYS
		};
		enum class bmsErrorCode
		{
			OK = 0,
			FILE_ERROR = 1,
			ALREADY_INITIALIZED,
			VALUE_ERROR,
			TYPE_MISMATCH,
			NOTE_LINE_ERROR,
		};
		enum class bmsGetChannelCode
		{
			BGM = 0,
			BPM,
			EXBPM,
			STOP,
			BGABASE,
			BGALAYER,
			BGAPOOR,
			NOTE1,
			NOTE2,
			NOTEINV1,
			NOTEINV2,
			NOTELN1,
			NOTELN2,
			NOTEMINE1,
			NOTEMINE2,
		};
	}

	// judge {PG, GR, GD, BD, PR, early poor}
	namespace judge
	{
		enum judgeDiff
		{
			VERYHARD = 0,
			HARD,
			NORMAL,
			EASY,
			VERYEASY,

			WHAT = 6
		};

		struct judge_t {
			unsigned PERFECT;
			unsigned GREAT;
			unsigned GOOD;
			unsigned BAD;
			unsigned POOR;
			unsigned BPOOR;
		};

		// Judge Time definitions.
		// Values are one-way judge times in ms, representing
		// PERFECT, GREAT, GOOD, BAD, POOR, ¿ÕPOOR respectively.
		const std::vector<judge_t> judgeTime = {
			{ 8, 27, 40, 150, 250, 600},		// VERY HARD
			{15, 32, 60, 200, 300, 600},		// HARD
			{18, 40, 96, 250, 400, 650},		// NORMAL
			{21, 60, 116, 320, 500, 800},		// EASY
			{},									// VERY EASY??
			{5, 10, 10, 150, 250, 600}			// XD
		};
		enum class judgeArea {
			BEFORE,
			BEFORE_BPOOR,
			BEFORE_BAD,
			BEFORE_GOOD,
			BEFORE_GREAT,
			BEFORE_PERFECT,
			AFTER_PERFECT,
			AFTER_GREAT,
			AFTER_GOOD,
			AFTER_BAD,
			AFTER_POOR,
			AFTER_KEY,
			AFTER,

			NOTHING
		};
	}
}
