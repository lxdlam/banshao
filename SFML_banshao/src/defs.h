#pragma once

namespace game::defs
{
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
	};;
}
