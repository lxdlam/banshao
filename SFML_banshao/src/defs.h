#pragma once
#include <utility>
#include <vector>

namespace game::defs
{
	// Mode
	enum class eMode{
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

	// bms related
	static const unsigned BGMCHANNELS = 32;
	static const unsigned MAXSAMPLEIDX = 36 * 36;
	static const unsigned MAXMEASUREIDX = 999;
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
		FILE_ERROR= 1,
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

	// judge {PG, GR, GD, BD, PR, early poor}
	struct judge_t {
		unsigned PERFECT;
		unsigned GREAT;
		unsigned GOOD;
		unsigned BAD;
		unsigned POOR;
		unsigned BPOOR;
	};
	const std::vector<judge_t> judge = {
		{ 8, 27, 40, 150, 250, 600},		// VERY HARD
		{15, 32, 60, 200, 300, 600},		// HARD
		{18, 40, 96, 250, 400, 650},		// NORMAL
		{21, 60, 116, 320, 500, 800},		// EASY
		{},									// VERY EASY??
		{5, 10, 10, 150, 250, 600}			// XD
	};
}
