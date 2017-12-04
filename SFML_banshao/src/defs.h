#pragma once
#include <string>
#include <array>
#include <vector>

using std::string;
namespace game { namespace defs

{
	namespace file
	{
		const string errorTextureImage = "resources/error.png";

		const string configFile = "config/config.json";
		const string keyconfigFile = "config/keyconfig.json";
	}

	namespace general
	{
		// Game 
		const string name = "asdf";
		const string subname = "alpha";
		const unsigned versionMajor = 0;
		const unsigned versionMinor = 1;

		// Mode
		enum class eMode {
			EXIT = 0,
			TITLE = 1,
			MUSIC_SELECT,
			DECIDE,
			THEME_SELECT,
			SOUNDSET,
			KEY_CONFIG,

			PLAY5,
			PLAY5_2,
			PLAY7,
			PLAY7_2,
			PLAY9,
			PLAY9_2,
			PLAY10,
			PLAY14,

			RESULT,
			COURSE_RESULT,

			TMPL,
			TEST,

			MODE_COUNT
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
			int PERFECT;
			int GREAT;
			int GOOD;
			int BAD;
			int BPOOR;
		};

		// Judge Time definitions.
		// Values are one-way judge times in ms, representing
		// PERFECT, GREAT, GOOD, BAD, 空POOR respectively.
		const std::vector<judge_t> judgeTime = {
			{ 8, 27, 40, 150, 600},		// VERY HARD
			{15, 32, 60, 200, 600},		// HARD
			{18, 40, 96, 250, 650},		// NORMAL
			{21, 60, 116, 320, 800},		// EASY
			{},									// VERY EASY??
			{5, 10, 10, 150, 600}			// XD
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
			AFTER,

			NOTHING
		};
	}

	// skin
	namespace skin
	{
		const size_t ERROR_IMAGE = 0;
		const size_t ERROR_TEXTURE = 0;

		enum specialGR
		{
			STAGEFILE = 101,
			BACKBMP,
			BANNER,
			SKIN_THUMBNAIL = 106,
			BLACK_DOT = 111,
			WHITE_DOT
		};

		enum class blend
		{
			TRANSCOLOR = 0,

			ALPHA = 1,
			ADD,
			SUBTRACT,
			MULTIPLY,

			XOR = 6,

			MULTIPLY_ANTI_BACKGROUND = 9,
			ANTI_COLOR = 10,
			MULTIPLY_WITH_ALPHA = 11

		};

		// unsigned
		enum class timer
		{
			SCENE_START = 0,
			INPUT_START,
			FADEOUT,
			STAGE_FAILED,
			//INPUT_END,

			LIST_MOVING = 10,
			SONG_CHANGE,
			LIST_MOVING_UP,
			LIST_MOVING_DOWN,
			LIST_STOP,
			README_START,
			README_STOP,
			COURSE_SELECT_START,
			COURSE_SELECT_STOP,

			// Option menu panels
			PANEL1_ON = 21,
			PANEL2_ON,
			PANEL3_ON,
			PANEL4_ON,
			PANEL5_ON,
			PANEL6_ON,
			PANEL7_ON,
			PANEL8_ON,
			PANEL9_ON,

			PANEL1_OFF = 31,
			PANEL2_OFF,
			PANEL3_OFF,
			PANEL4_OFF,
			PANEL5_OFF,
			PANEL6_OFF,
			PANEL7_OFF,
			PANEL8_OFF,
			PANEL9_OFF,

			LOAD_FINISH = 40,
			PLAY_START,
			HP_UP_1P,
			HP_UP_2P,
			HP_MAX_1P,
			HP_MAX_2P,
			JUDGE_1P,
			JUDGE_2P,
			FULLCOMBO_1P,
			FULLCOMBO_2P,

			// bomb
			HITEFFECT_SCR_1P = 50,
			HITEFFECT_1_1P,
			HITEFFECT_2_1P,
			HITEFFECT_3_1P,
			HITEFFECT_4_1P,
			HITEFFECT_5_1P,
			HITEFFECT_6_1P,
			HITEFFECT_7_1P,
			HITEFFECT_8_1P,
			HITEFFECT_9_1P,

			HITEFFECT_SCR_2P = 60,
			HITEFFECT_1_2P,
			HITEFFECT_2_2P,
			HITEFFECT_3_2P,
			HITEFFECT_4_2P,
			HITEFFECT_5_2P,
			HITEFFECT_6_2P,
			HITEFFECT_7_2P,
			HITEFFECT_8_2P,
			HITEFFECT_9_2P,

			LONGEFFECT_SCR_1P = 70,
			LONGEFFECT_1_1P,
			LONGEFFECT_2_1P,
			LONGEFFECT_3_1P,
			LONGEFFECT_4_1P,
			LONGEFFECT_5_1P,
			LONGEFFECT_6_1P,
			LONGEFFECT_7_1P,
			LONGEFFECT_8_1P,
			LONGEFFECT_9_1P,

			LONGEFFECT_SCR_2P = 80,
			LONGEFFECT_1_2P,
			LONGEFFECT_2_2P,
			LONGEFFECT_3_2P,
			LONGEFFECT_4_2P,
			LONGEFFECT_5_2P,
			LONGEFFECT_6_2P,
			LONGEFFECT_7_2P,
			LONGEFFECT_8_2P,
			LONGEFFECT_9_2P,

			KEY_ON_SCR_1P = 100,
			KEY_ON_1_1P,
			KEY_ON_2_1P,
			KEY_ON_3_1P,
			KEY_ON_4_1P,
			KEY_ON_5_1P,
			KEY_ON_6_1P,
			KEY_ON_7_1P,
			KEY_ON_8_1P,
			KEY_ON_9_1P,

			KEY_ON_SCR_2P = 110,
			KEY_ON_1_2P,
			KEY_ON_2_2P,
			KEY_ON_3_2P,
			KEY_ON_4_2P,
			KEY_ON_5_2P,
			KEY_ON_6_2P,
			KEY_ON_7_2P,
			KEY_ON_8_2P,
			KEY_ON_9_2P,

			KEY_OFF_SCR_1P = 120,
			KEY_OFF_1_1P,
			KEY_OFF_2_1P,
			KEY_OFF_3_1P,
			KEY_OFF_4_1P,
			KEY_OFF_5_1P,
			KEY_OFF_6_1P,
			KEY_OFF_7_1P,
			KEY_OFF_8_1P,
			KEY_OFF_9_1P,

			KEY_OFF_SCR_2P = 130,
			KEY_OFF_1_2P,
			KEY_OFF_2_2P,
			KEY_OFF_3_2P,
			KEY_OFF_4_2P,
			KEY_OFF_5_2P,
			KEY_OFF_6_2P,
			KEY_OFF_7_2P,
			KEY_OFF_8_2P,
			KEY_OFF_9_2P,

			BEAT_PROGRESS = 140,

			LAST_NOTE_JUDGE_1P = 143,
			LAST_NOTE_JUDGE_2P,

			RESULT_START = 150,
			RESULT_RANK,
			RESULT_HIGHSCORE,

			DATABASE_LOADING = 170,
			DATABASE_LOAD_END,

			IR_CONNECT = 172,
			IR_SUCCESS,
			IR_FAILED,

			COURSE_EDITOR_START = 180,
			COURSE_EDITOR_END,

			TIMER_COUNT
		};
		
		// bool
		enum class dst_option
		{
			TRUE = 0,

			// song select
			SELECT_SELECTING_FOLDER = 1,
			SELECT_SELECTING_SONG,
			SELECT_SELECTING_COURSE,
			SELECT_SELECTING_NEW_COURSE,
			SELECT_SELECTING_PLAYABLE,

			// mode
			DOUBLE = 10,
			BATTLE,
			DOUBLE_OR_BATTLE,
			GHOST_OR_BATTLE,

			NO_PANEL_ON = 20,
			PANEL1_ON,
			PANEL2_ON,
			PANEL3_ON,
			PANEL4_ON,
			PANEL5_ON,
			PANEL6_ON,
			PANEL7_ON,
			PANEL8_ON,
			PANEL9_ON,

			// system settings
			SYSTEM_BGA_NORMAL = 30,
			SYSTEM_BGA_EXTEND,
			SYSTEM_AUTOPLAY_OFF,
			SYSTEM_AUTOPLAY_ON,
			SYSTEM_GHOST_OFF,
			SYSTEM_GHOST_TYPE_A,
			SYSTEM_GHOST_TYPE_B,
			SYSTEM_GHOST_TYPE_C,
			SYSTEM_SCOREGRAPH_OFF,
			SYSTEM_SCOREGRAPH_ON,
			SYSTEM_BGA_OFF,
			SYSTEM_BGA_ON,

			NORMAL_GAUGE_1P,
			HARD_GAUGE_1P,
			NORMAL_GAUGE_2P,
			HARD_GAUGE_2P,

			DIFF_FILTER_ON,
			DIFF_FILTER_OFF,

			// infos
			OFFLINE = 50,
			ONLINE,

			EXTRA_MODE_OFF,
			EXTRA_MODE_ON,

			AUTO_SCRATCH_1P_OFF,
			AUTO_SCRATCH_1P_ON,
			AUTO_SCRATCH_2P_OFF,
			AUTO_SCRATCH_2P_ON,

			SCORE_NOSAVE = 60,
			SCORE_SAVE,
			CLEAR_NOSAVE,
			CLEAR_SAVE_EASY,
			CLEAR_SAVE_NORMAL,
			CLEAR_SAVE_HARD,
			CLEAR_FULLCOMBO_ONLY,

			LEVEL_INRANGE_BEGINNER = 70,
			LEVEL_INRANGE_NORMAL,
			LEVEL_INRANGE_HYPER,
			LEVEL_INRANGE_ANOTHER,
			LEVEL_INRANGE_INSANE,
			LEVEL_OVERFLOW_BEGINNER,
			LEVEL_OVERFLOW_NORMAL,
			LEVEL_OVERFLOW_HYPER,
			LEVEL_OVERFLOW_ANOTHER,
			LEVEL_OVERFLOW_INSANE,

			LOADING = 80,
			LOAD_FINISHED,

			REPLAY_OFF = 82,
			REPLAY_RECORDING,
			REPLAY_PLAYING,

			RESULT_CLEAR = 90,
			RESULT_FAILED,

			// song list status
			CHART_NOPLAY = 100,
			CHART_FAILED,
			CHART_EASY,
			CHART_NORMAL,
			CHART_HARD,
			CHART_FULLCOMBO,

			CHART_AAA = 110,
			CHART_AA,
			CHART_A,
			CHART_B,
			CHART_C,
			CHART_D,
			CHART_E,
			CHART_F,

			// Clear option flag
			CHART_CLEARED_GAUGE_NORMAL = 118,
			CHART_CLEARED_GAUGE_HARD,
			CHART_CLEARED_GAUGE_DEATH,
			CHART_CLEARED_GAUGE_EASY,
			CHART_CLEARED_GAUGE_PATTACK,
			CHART_CLEARED_GAUGE_GATTACK,

			CHART_CLEARED_SHUFFLE_OFF = 126,
			CHART_CLEARED_SHUFFLE_MIRROR,
			CHART_CLEARED_SHUFFLE_RANDOM,
			CHART_CLEARED_SHUFFLE_SRANDOM,
			CHART_CLEARED_SHUFFLE_HRANDOM,
			CHART_CLEARED_SHUFFLE_ALLSCR,

			CHART_CLEARED_EFFECT_OFF = 134,
			CHART_CLEARED_EFFECT_HIDDEN,
			CHART_CLEARED_EFFECT_SUDDEN,
			CHART_CLEARED_EFFECT_HIDSUD,

			CHART_CLEARED_ETC_AUTOSCR = 142,
			CHART_CLEARED_ETC_EXTRAMODE,
			CHART_CLEARED_ETC_DOUBLEBATTLE,
			CHART_CLEARED_ETC_SPTODP,

			// metadata
			CHART_DIFF_NONSET = 150,
			CHART_DIFF_EASY,
			CHART_DIFF_NORMAL,
			CHART_DIFF_HYPER,
			CHART_DIFF_ANOTHER,
			CHART_DIFF_INSANE,

			CHART_MODE_7KEYS = 160,
			CHART_MODE_5KEYS,
			CHART_MODE_14KEYS,
			CHART_MODE_10KEYS,
			CHART_MODE_9KEYS,

			// play mode after applying mods
			CHART_PLAYMODE_7KEYS = 165,
			CHART_PLAYMODE_5KEYS,
			CHART_PLAYMODE_14KEYS,
			CHART_PLAYMODE_10KEYS,
			CHART_PLAYMODE_9KEYS,

			CHART_NO_BGA = 170,
			CHART_HAVE_BGA,
			CHART_NO_LN,
			CHART_HAVE_LN,
			CHART_NO_README,
			CHART_HAVE_README,
			CHART_NO_BPMCHANGE,
			CHART_HAVE_BPMCHANGE,
			CHART_NO_RANDOM,
			CHART_HAVE_RANDOM,

			CHART_JUDGE_VHARD = 180,
			CHART_JUDGE_HARD,
			CHART_JUDGE_NORMAL,
			CHART_JUDGE_EASY,

			CHART_LEVEL_INRANGE = 185,
			CHART_LEVEL_OVERFLOW,

			CHART_NO_STAGEFILE = 190,
			CHART_HAVE_STAGEFILE,
			CHART_NO_BANNER,
			CHART_HAVE_BANNER,
			CHART_NO_BACKBMP,
			CHART_HAVE_BACKBMP,

			CHART_NO_REPLAY,
			CHART_HAVE_REPLAY,

			// Playing
			PLAY_AAA_1P = 200,
			PLAY_AA_1P,
			PLAY_A_1P,
			PLAY_B_1P,
			PLAY_C_1P,
			PLAY_D_1P,
			PLAY_E_1P,
			PLAY_F_1P,

			PLAY_AAA_2P = 210,
			PLAY_AA_2P,
			PLAY_A_2P,
			PLAY_B_2P,
			PLAY_C_2P,
			PLAY_D_2P,
			PLAY_E_2P,
			PLAY_F_2P,

			PLAY_ABOVE_AAA = 220,
			PLAY_ABOVE_AA,
			PLAY_ABOVE_A,
			PLAY_ABOVE_B,
			PLAY_ABOVE_C,
			PLAY_ABOVE_D,
			PLAY_ABOVE_E,
			PLAY_ABOVE_F,

			PLAY_0p_TO_10p_1P = 230,
			PLAY_10p_TO_19p_1P,
			PLAY_20p_TO_29p_1P,
			PLAY_30p_TO_39p_1P,
			PLAY_40p_TO_49p_1P,
			PLAY_50p_TO_59p_1P,
			PLAY_60p_TO_69p_1P,
			PLAY_70p_TO_79p_1P,
			PLAY_80p_TO_89p_1P,
			PLAY_90p_TO_99p_1P,
			PLAY_100p_1P,

			PLAY_PERFECT_1P = 241,
			PLAY_GREAT_1P,
			PLAY_GOOD_1P,
			PLAY_BAD_1P,
			PLAY_POOR_1P,
			PLAY_BPOOR_1P,

			//公式ハーフスキンの左右のネオン用です 2P側も
			PLAY_POORBGA_NOT_INTIME_1P = 247,
			PLAY_POORBGA_INTIME_1P,

			PLAY_0p_TO_10p_2P = 250,
			PLAY_10p_TO_19p_2P,
			PLAY_20p_TO_29p_2P,
			PLAY_30p_TO_39p_2P,
			PLAY_40p_TO_49p_2P,
			PLAY_50p_TO_59p_2P,
			PLAY_60p_TO_69p_2P,
			PLAY_70p_TO_79p_2P,
			PLAY_80p_TO_89p_2P,
			PLAY_90p_TO_99p_2P,
			PLAY_100p_2P,

			PLAY_PERFECT_2P = 261,
			PLAY_GREAT_2P,
			PLAY_GOOD_2P,
			PLAY_BAD_2P,
			PLAY_POOR_2P,
			PLAY_BPOOR_2P,

			PLAY_POORBGA_NOT_INTIME_2P = 267,
			PLAY_POORBGA_INTIME_2P,

			PLAY_CHANGING_LANECOVER_1P = 270,
			PLAY_CHANGING_LANECOVER_2P,

			PLAY_STAGE1 = 280,
			PLAY_STAGE2,
			PLAY_STAGE3,
			PLAY_STAGE4,
			//PLAY_STAGE5,
			//PLAY_STAGE6,
			//PLAY_STAGE7,
			//PLAY_STAGE8,
			//PLAY_STAGE9,
			PLAY_STAGEFINAL = 289,

			PLAY_MODE_COURSE = 290,
			PLAY_MODE_NONSTOP,
			PLAY_MODE_EXHARD,
			PLAY_MODE_SKILLANALYSIS,

			// Result
			RESULT_AAA_1P = 300,
			RESULT_AA_1P,
			RESULT_A_1P,
			RESULT_B_1P,
			RESULT_C_1P,
			RESULT_D_1P,
			RESULT_E_1P,
			RESULT_F_1P,
			RESULT_0_1P,

			RESULT_AAA_2P = 310,
			RESULT_AA_2P,
			RESULT_A_2P,
			RESULT_B_2P,
			RESULT_C_2P,
			RESULT_D_2P,
			RESULT_E_2P,
			RESULT_F_2P,
			RESULT_0_2P,

			RESULT_MYBEST_AAA = 320,
			RESULT_MYBEST_AA,
			RESULT_MYBEST_A,
			RESULT_MYBEST_B,
			RESULT_MYBEST_C,
			RESULT_MYBEST_D,
			RESULT_MYBEST_E,
			RESULT_MYBEST_F,

			RESULT_スコアが更新された = 330,
			RESULT_MAXCOMBOが更新された,
			RESULT_最小BPが更新された,
			RESULT_トライアルが更新された,
			RESULT_IRの順位が更新された,
			RESULT_スコアランクが更新された,

			RESULT_UPDATE_AAA = 340,
			RESULT_UPDATE_AA,
			RESULT_UPDATE_A,
			RESULT_UPDATE_B,
			RESULT_UPDATE_C,
			RESULT_UPDATE_D,
			RESULT_UPDATE_E,
			RESULT_UPDATE_F,

			RESULT_リザルトフリップ無効 = 350,
			RESULT_リザルトフリップ有効,

			RESULT_1PWIN_2PLOSE = 352,
			RESULT_1PLOSE_2PWIN,
			RESULT_DRAW,

			// Key config
			KEYCONFIG_7KEYS = 400,
			KEYCONFIG_9KEYS,
			KEYCONFIG_5KEYS,

			// Others
			SELECT_NO_BEGINNER_IN_SAME_FOLDER = 500,
			SELECT_NO_NORMAL_IN_SAME_FOLDER,
			SELECT_NO_HYPER_IN_SAME_FOLDER,
			SELECT_NO_ANOTHER_IN_SAME_FOLDER,
			SELECT_NO_INSANE_IN_SAME_FOLDER,

			SELECT_HAVE_BEGINNER_IN_SAME_FOLDER = 505,
			SELECT_HAVE_NORMAL_IN_SAME_FOLDER,
			SELECT_HAVE_HYPER_IN_SAME_FOLDER,
			SELECT_HAVE_ANOTHER_IN_SAME_FOLDER,
			SELECT_HAVE_INSANE_IN_SAME_FOLDER,

			SELECT_HAVE_ONE_BEGINNER_IN_SAME_FOLDER = 510,
			SELECT_HAVE_ONE_NORMAL_IN_SAME_FOLDER,
			SELECT_HAVE_ONE_HYPER_IN_SAME_FOLDER,
			SELECT_HAVE_ONE_ANOTHER_IN_SAME_FOLDER,
			SELECT_HAVE_ONE_NSANE_IN_SAME_FOLDER,

			SELECT_HAVE_COUPLE_BEGINNER_IN_SAME_FOLDER = 515,
			SELECT_HAVE_COUPLE_NORMAL_IN_SAME_FOLDER,
			SELECT_HAVE_COUPLE_HYPER_IN_SAME_FOLDER,
			SELECT_HAVE_COUPLE_ANOTHER_IN_SAME_FOLDER,
			SELECT_HAVE_COUPLE_NSANE_IN_SAME_FOLDER,

			SELECT_SCOREBAR_BEGINNER_NO_PLAY = 520,
			SELECT_SCOREBAR_BEGINNER_FAILED,
			SELECT_SCOREBAR_BEGINNER_EASY,
			SELECT_SCOREBAR_BEGINNER_CLEAR,
			SELECT_SCOREBAR_BEGINNER_HARDCLEAR,
			SELECT_SCOREBAR_BEGINNER_FULLCOMBO,

			SELECT_SCOREBAR_NORMAL_NO_PLAY = 530,
			SELECT_SCOREBAR_NORMAL_FAILED,
			SELECT_SCOREBAR_NORMAL_EASY,
			SELECT_SCOREBAR_NORMAL_CLEAR,
			SELECT_SCOREBAR_NORMAL_HARDCLEAR,
			SELECT_SCOREBAR_NORMAL_FULLCOMBO,

			SELECT_SCOREBAR_HYPER_NO_PLAY = 540,
			SELECT_SCOREBAR_HYPER_FAILED,
			SELECT_SCOREBAR_HYPER_EASY,
			SELECT_SCOREBAR_HYPER_CLEAR,
			SELECT_SCOREBAR_HYPER_HARDCLEAR,
			SELECT_SCOREBAR_HYPER_FULLCOMBO,

			SELECT_SCOREBAR_ANOTHER_NO_PLAY = 550,
			SELECT_SCOREBAR_ANOTHER_FAILED,
			SELECT_SCOREBAR_ANOTHER_EASY,
			SELECT_SCOREBAR_ANOTHER_CLEAR,
			SELECT_SCOREBAR_ANOTHER_HARDCLEAR,
			SELECT_SCOREBAR_ANOTHER_FULLCOMBO,

			SELECT_SCOREBAR_INSANE_NO_PLAY = 560,
			SELECT_SCOREBAR_INSANE_FAILED,
			SELECT_SCOREBAR_INSANE_EASY,
			SELECT_SCOREBAR_INSANE_CLEAR,
			SELECT_SCOREBAR_INSANE_HARDCLEAR,
			SELECT_SCOREBAR_INSANE_FULLCOMBO,


			/////////////////////////////////////
			//　コースセレクト関連
			COURSE_STAGE_MORE_THAN_1 = 580,
			COURSE_STAGE_MORE_THAN_2,
			COURSE_STAGE_MORE_THAN_3,
			COURSE_STAGE_MORE_THAN_4,
			COURSE_STAGE_MORE_THAN_5,
			COURSE_STAGE_MORE_THAN_6,
			COURSE_STAGE_MORE_THAN_7,
			COURSE_STAGE_MORE_THAN_8,
			COURSE_STAGE_MORE_THAN_9,
			COURSE_STAGE_MORE_THAN_10,

			COURSESELECT_SELECTING_STAGE1 = 590,
			COURSESELECT_SELECTING_STAGE2,
			COURSESELECT_SELECTING_STAGE3,
			COURSESELECT_SELECTING_STAGE4,
			COURSESELECT_SELECTING_STAGE5,
			COURSESELECT_SELECTING_STAGE6,
			COURSESELECT_SELECTING_STAGE7,
			COURSESELECT_SELECTING_STAGE8,
			COURSESELECT_SELECTING_STAGE9,
			COURSESELECT_SELECTING_STAGE10,

			MODE_COURSESELECT = 571,
			MODE_NOT_COURSESELECT,

			//COURSE_STAGE1
			COURSE_STAGE1_DIFF_NONSET = 700,
			COURSE_STAGE1_DIFF_BEGINNER,
			COURSE_STAGE1_DIFF_NORMAL,
			COURSE_STAGE1_DIFF_HYPER,
			COURSE_STAGE1_DIFF_ANOTHER,
			COURSE_STAGE1_DIFF_INSANE,

			//COURSE_STAGE2
			COURSE_STAGE2_DIFF_NONSET = 710,
			COURSE_STAGE2_DIFF_BEGINNER,
			COURSE_STAGE2_DIFF_NORMAL,
			COURSE_STAGE2_DIFF_HYPER,
			COURSE_STAGE2_DIFF_ANOTHER,
			COURSE_STAGE2_DIFF_INSANE,

			//COURSE_STAGE3
			COURSE_STAGE3_DIFF_NONSET = 720,
			COURSE_STAGE3_DIFF_BEGINNER,
			COURSE_STAGE3_DIFF_NORMAL,
			COURSE_STAGE3_DIFF_HYPER,
			COURSE_STAGE3_DIFF_ANOTHER,
			COURSE_STAGE3_DIFF_INSANE,

			//COURSE_STAGE4
			COURSE_STAGE4_DIFF_NONSET = 730,
			COURSE_STAGE4_DIFF_BEGINNER,
			COURSE_STAGE4_DIFF_NORMAL,
			COURSE_STAGE4_DIFF_HYPER,
			COURSE_STAGE4_DIFF_ANOTHER,
			COURSE_STAGE4_DIFF_INSANE,

			//COURSE_STAGE5
			COURSE_STAGE5_DIFF_NONSET = 740,
			COURSE_STAGE5_DIFF_BEGINNER,
			COURSE_STAGE5_DIFF_NORMAL,
			COURSE_STAGE5_DIFF_HYPER,
			COURSE_STAGE5_DIFF_ANOTHER,
			COURSE_STAGE5_DIFF_INSANE,

			//LR2IR
			IR_対象ではない = 600,  //???
			IR_LOADING,
			IR_LOAD_COMPLETE,
			IR_NO_PLAYER,
			IR_FAILED,
			IR_BANNED_CHART,
			IR_WAITING_FOR_UPDATE,
			IR_ACCESS,
			IR_BUSY,

			IR_NOT_SHOWING_RANKING = 620,
			IR_SHOWING_RANKING,

			IR_NOT_IN_GHOSTBATTLE = 622,
			IR_IN_GHOSTBATTLE,

			IR_自分と相手のスコアを比較する状況ではない = 624,	//現状では、ランキング表示中とライバルフォルダ
			IR_自分と相手のスコアを比較するべき状況である,

			IR_RIVAL_NOPLAY = 640,
			IR_RIVAL_FAILED,
			IR_RIVAL_EASY_CLEARED,
			IR_RIVAL_NORMAL_CLEARED,
			IR_RIVAL_HARD_CLEARED,
			IR_RIVAL_FULL_COMBO,

			IR_RIVAL_AAA = 650,
			IR_RIVAL_AA,
			IR_RIVAL_A,
			IR_RIVAL_B,
			IR_RIVAL_C,
			IR_RIVAL_D,
			IR_RIVAL_E,
			IR_RIVAL_F,

			FALSE = 999,

			DST_OPTION_COUNT
		};
		
		// int
		enum class num
		{

			// play options
			HISPEED_1P = 10,
			HISPEED_2P = 11,

			JUDGE_TIMING = 12,
			DEFAULT_TARGET_RATE = 13,

			LANECOVER_1P = 14,
			LANECOVER_2P = 15,

			// random stuff
			FPS = 20,

			YEAR = 21,
			MONTH = 22,
			DAY = 23,
			HOUR = 24,
			MIN = 25,
			SEC = 26,

			// Player Status

			TOTAL_PLAY_COUNT = 30,
			TOTAL_CLEAR_COUNT,
			TOTAL_FAIL_COUNT,

			TOTAL_PERFECT,
			TOTAL_GREAT,
			TOTAL_GOOD,
			TOTAL_BAD,
			TOTAL_POOR,

			RUNNING_COMBO,
			RUNNING_COMBO_MAX,

			TRIAL_LEVEL,
			TRIAL_LEVEL_MINUS_1,

			LEVEL_OF_BEGINNER_CHART_FROM_SAME_FOLDER = 45,
			LEVEL_OF_NORMAL_CHART_FROM_SAME_FOLDER,
			LEVEL_OF_HYPER_CHART_FROM_SAME_FOLDER,
			LEVEL_OF_ANOTHER_CHART_FROM_SAME_FOLDER,
			LEVEL_OF_INSANE_CHART_FROM_SAME_FOLDER,

			// Effector
			EQ0 = 50,
			EQ1,
			EQ2,
			EQ3,
			EQ4,
			EQ5,
			EQ6,

			MASTER_VOLUME,
			KEY_VOLUME,
			BGM_VOLUME,

			FX0_P1,
			FX0_P2,
			FX1_P1,
			FX1_P2,
			FX2_P1,
			FX2_P2,

			PITCH,

			//選曲時,
			SELECT_SCORE = 70,
			SELECT_EXSCORE,
			SELECT_MAX_EXSCORE,
			SELECT_RATE,
			SELECT_TOTALNOTES,
			SELECT_MAXCOMBO,
			SELECT_MIN_BP,
			SELECT_PLAYCOUNT,
			SELECT_CLEAR,
			SELECT_FAIL,

			SELECT_PERFECT,
			SELECT_GREAT,
			SELECT_GOOD,
			SELECT_BAD,
			SELECT_POOR,
			SELECT_PERFECT_PERCENT,
			SELECT_GREAT_PERCENT,
			SELECT_GOOD_PERCENT,
			SELECT_BAD_PERCENT,
			SELECT_POOR_PERCENT,

			SELECT_BPM_MAX,
			SELECT_BPM_MIN,

			SELECT_IR_RANK,
			SELECT_IR_TOTALPLAYER,
			SELECT_IR_CLEARRATE,

			SELECT_IR_RIVAL_DELTA,

			//BMSプレイ時,

			//1P,
			SCORE = 100,
			EXSCORE,
			RATE,
			RATE_DECIMAL2,
			NOWCOMBO,
			MAXCOMBO,
			TOTALNOTES,
			GROOVEGAUGE,
			EXSCORE_2P_DELTA,
			PERFECT,
			GREAT,
			GOOD,
			BAD,
			POOR,
			TOTAL_RATE,
			TOTAL_RATE_DECIMAL2,

			//対戦相手ORゴースト,
			RIVAL_SCORE = 120,
			RIVAL_EXSCORE,
			RIVAL_RATE,
			RIVAL_RATE_DECIMAL2,
			RIVAL_NOWCOMBO,
			RIVAL_MAXCOMBO,
			RIVAL_TOTALNOTES,
			RIVAL_GROOVEGAUGE,
			RIVAL_EXSCORE_2P_DELTA,
			RIVAL_PERFECT,
			RIVAL_GREAT,
			RIVAL_GOOD,
			RIVAL_BAD,
			RIVAL_POOR,
			RIVAL_TOTAL_RATE,
			RIVAL_TOTAL_RATE_DECIMAL2,

			//120-139は設定によってハイスコアかゴーストか不定なので、スコアグラフ上で指定して表示する場合,
			//リザルトで使用する場合は#DISABLEFLIP必須,
			ハイスコア現在値 = 150,
			ターゲット現在値,
			ハイスコアと1Pスコアの差,
			ターゲットと1Pスコアの差,
			次のランクとの差,
			ハイスコアRATE,
			ハイスコアRATE小数点2桁,
			ターゲットRATE,
			ターゲットRATE小数点2桁,

			//BMSの状態,
			BPM = 160,
			BMS_MIN,
			BMS_SEC,
			BMS_REMAIN_MIN,
			BMS_REMAIN_SEC,
			LOAD_PROGRESS,

			//リザのハイスコア表示用,
			RESULT_EXSCORE_MYBEST = 170,
			RESULT_EXSCORE,
			RESULT_EXSCORE_DELTA,
			
			RESULT_MAXCOMBO_MYBEST,
			RESULT_MAXCOMBO,
			RESULT_MAXCOMBO_DELTA,
			
			RESULT_BP_MYBEST,
			RESULT_BP,
			RESULT_BP_DELTA,
			
			RESULT_IR_RANK,
			RESULT_IR_TOTALPLAYER,
			RESULT_IR_CLEARRATE,
			RESULT_IR_RANK__MYBEST,
			
			RESULT_RATE_MYBEST,
			RESULT_RATE_MYBEST_DECIMAL2,

			//IR(BETA3以降用),

			IR_TOTALPLAYER = 200,
			IR_TOTALPLAYS__FASTSLOW_JUDGE_DELTA,

			FAILED_PLAYER_COUNT__FASTSLOW = 210,
			FAILED_PROPORTION,
			EASY_PLAYER_COUNT__FAST_COUNT,
			EASY_PROPORTION,
			CLEAR_PLAYER_COUNT__SLOW_COUNT,
			CLEAR_PROPORTION,
			HARD_PLAYER_COUNT,
			HARD_PROPORTION,
			FULLCOMBO_PLAYER_COUNT,
			FULLCOMBO_PROPORTION,

			IR自動更新までの残り時間 = 220,

			COURSE_STAGE1 = 250,
			COURSE_STAGE2,
			COURSE_STAGE3,
			COURSE_STAGE4,
			COURSE_STAGE5,


			//選曲時ライバル,
			SELECT_RIVAL_SCORE = 270,
			SELECT_RIVAL_EXSCORE,
			SELECT_RIVAL_MAX_EXSCORE,
			SELECT_RIVAL_RATE,
			SELECT_RIVAL_TOTALNOTES,
			SELECT_RIVAL_MAXCOMBO,
			SELECT_RIVAL_MIN_BP,
			SELECT_RIVAL_PLAYCOUNT,
			SELECT_RIVAL_CLEAR,
			SELECT_RIVAL_FAIL,
			SELECT_RIVAL_PERFECT,
			SELECT_RIVAL_GREAT,
			SELECT_RIVAL_GOOD,
			SELECT_RIVAL_BAD,
			SELECT_RIVAL_POOR,
			SELECT_RIVAL_PERFECT_PERCENT,
			SELECT_RIVAL_GREAT_PERCENT,
			SELECT_RIVAL_GOOD_PERCENT,
			SELECT_RIVAL_BAD_PERCENT,
			SELECT_RIVAL_POOR_PERCENT,

			SELECT_RIVAL_BPM_MAX,
			SELECT_RIVAL_BPM_MIN,

			SELECT_RIVAL_IR_RANK,
			SELECT_RIVAL_IR_TOTALPLAYER,
			SELECT_RIVAL_IR_CLEARRATE,

			NUM_COUNT
		};

		// unsigned
		enum class slider
		{
			SELECT_POSITION = 1,

			HISPEED_1P = 2,
			HISPEED_2P,

			LANECOVER_1P = 4,
			LANECOVER_2P,

			SONG_PROGRESS = 6,

			SKINSELECT_CUSTOMIZE = 7,

			IR_POSITION = 8,

			EQ0 = 10,
			EQ1,
			EQ2,
			EQ3,
			EQ4,
			EQ5,
			EQ6,

			VOLUME_MASTER = 17,
			VOLUME_KEY,
			VOLUME_BGM,

			FX_MASTER_P1 = 20,
			FX_MASTER_P2,
			FX_KEY_P1,
			FX_KEY_P2,
			FX_BGM_P1,
			FX_BGM_P2,
			FX_PITCH,

			SLIDER_COUNT
		};

		enum class elementType
		{
			GENERAL,
			NUMBER,
			SLIDER,
			BARGRAPH,
			BUTTON,
			ONMOUSE,
			TEXT
		};
	}

	namespace scene
	{
		using skin::dst_option;
		using skin::timer;
		struct setDst { dst_option dstIdx; bool val; };
		static const std::array<std::vector<timer>, key::GAMEPAD_KEY_COUNT> gamepadTimerPressMap
		{ {
			{timer::KEY_ON_SCR_1P},	// 1P Scratch L
			{timer::KEY_ON_SCR_1P}, // 1P Scratch R
			{timer::PANEL1_ON, timer::KEY_ON_1_1P},
			{timer::PANEL2_ON, timer::KEY_ON_2_1P},
			{timer::PANEL3_ON, timer::KEY_ON_3_1P},
			{timer::PANEL4_ON, timer::KEY_ON_4_1P},
			{timer::PANEL5_ON, timer::KEY_ON_5_1P},
			{timer::PANEL6_ON, timer::KEY_ON_6_1P},
			{timer::PANEL7_ON, timer::KEY_ON_7_1P},
			{timer::PANEL8_ON, timer::KEY_ON_8_1P},
			{timer::PANEL9_ON, timer::KEY_ON_9_1P},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{}, // 2P Scratch R
			{timer::PANEL1_ON, timer::KEY_ON_1_2P},
			{timer::PANEL2_ON, timer::KEY_ON_2_2P},
			{timer::PANEL3_ON, timer::KEY_ON_3_2P},
			{timer::PANEL4_ON, timer::KEY_ON_4_2P},
			{timer::PANEL5_ON, timer::KEY_ON_5_2P},
			{timer::PANEL6_ON, timer::KEY_ON_6_2P},
			{timer::PANEL7_ON, timer::KEY_ON_7_2P},
			{timer::PANEL8_ON, timer::KEY_ON_8_2P},
			{timer::PANEL9_ON, timer::KEY_ON_9_2P},
			{timer::KEY_ON_SCR_2P},	// 2P Start
			{timer::KEY_ON_SCR_2P},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		static const std::array<std::vector<setDst>, key::GAMEPAD_KEY_COUNT> gamepadDstOptPressMap
		{ {
			{},	// 1P Scratch L
			{}, // 1P Scratch R
			{{dst_option::PANEL1_ON, true}},
			{{dst_option::PANEL2_ON, true}},
			{{dst_option::PANEL3_ON, true}},
			{{dst_option::PANEL4_ON, true}},
			{{dst_option::PANEL5_ON, true}},
			{{dst_option::PANEL6_ON, true}},
			{{dst_option::PANEL7_ON, true}},
			{{dst_option::PANEL8_ON, true}},
			{{dst_option::PANEL9_ON, true}},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{},	// 2P Scratch R
			{{dst_option::PANEL1_ON, true}},
			{{dst_option::PANEL2_ON, true}},
			{{dst_option::PANEL3_ON, true}},
			{{dst_option::PANEL4_ON, true}},
			{{dst_option::PANEL5_ON, true}},
			{{dst_option::PANEL6_ON, true}},
			{{dst_option::PANEL7_ON, true}},
			{{dst_option::PANEL8_ON, true}},
			{{dst_option::PANEL9_ON, true}},
			{},	// 2P Start
			{},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		static const std::array<std::vector<timer>, key::GAMEPAD_KEY_COUNT> gamepadTimerReleaseMap
		{ {
			{timer::KEY_OFF_SCR_1P},	// 1P Scratch L
			{timer::KEY_OFF_SCR_1P}, // 1P Scratch R
			{timer::PANEL1_OFF, timer::KEY_OFF_1_1P},
			{timer::PANEL2_OFF, timer::KEY_OFF_2_1P},
			{timer::PANEL3_OFF, timer::KEY_OFF_3_1P},
			{timer::PANEL4_OFF, timer::KEY_OFF_4_1P},
			{timer::PANEL5_OFF, timer::KEY_OFF_5_1P},
			{timer::PANEL6_OFF, timer::KEY_OFF_6_1P},
			{timer::PANEL7_OFF, timer::KEY_OFF_7_1P},
			{timer::PANEL8_OFF, timer::KEY_OFF_8_1P},
			{timer::PANEL9_OFF, timer::KEY_OFF_9_1P},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{}, // 2P Scratch R
			{timer::PANEL1_OFF, timer::KEY_OFF_1_2P},
			{timer::PANEL2_OFF, timer::KEY_OFF_2_2P},
			{timer::PANEL3_OFF, timer::KEY_OFF_3_2P},
			{timer::PANEL4_OFF, timer::KEY_OFF_4_2P},
			{timer::PANEL5_OFF, timer::KEY_OFF_5_2P},
			{timer::PANEL6_OFF, timer::KEY_OFF_6_2P},
			{timer::PANEL7_OFF, timer::KEY_OFF_7_2P},
			{timer::PANEL8_OFF, timer::KEY_OFF_8_2P},
			{timer::PANEL9_OFF, timer::KEY_OFF_9_2P},
			{timer::KEY_OFF_SCR_2P},	// 2P Start
			{timer::KEY_OFF_SCR_2P},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
		static const std::array<std::vector<setDst>, key::GAMEPAD_KEY_COUNT> gamepadDstOptReleaseMap
		{ {
			{},	// 1P Scratch L
			{}, // 1P Scratch R
			{{dst_option::PANEL1_ON, false}},
			{{dst_option::PANEL2_ON, false}},
			{{dst_option::PANEL3_ON, false}},
			{{dst_option::PANEL4_ON, false}},
			{{dst_option::PANEL5_ON, false}},
			{{dst_option::PANEL6_ON, false}},
			{{dst_option::PANEL7_ON, false}},
			{{dst_option::PANEL8_ON, false}},
			{{dst_option::PANEL9_ON, false}},
			{},	// 1P Start
			{},	// 1P Select
			{},	// 1P SpeedUp
			{},	// 1P SpeedDown
			{},	// 2P Scratch L
			{},	// 2P Scratch R
			{{dst_option::PANEL1_ON, false}},
			{{dst_option::PANEL2_ON, false}},
			{{dst_option::PANEL3_ON, false}},
			{{dst_option::PANEL4_ON, false}},
			{{dst_option::PANEL5_ON, false}},
			{{dst_option::PANEL6_ON, false}},
			{{dst_option::PANEL7_ON, false}},
			{{dst_option::PANEL8_ON, false}},
			{{dst_option::PANEL9_ON, false}},
			{},	// 2P Start
			{},	// 2P Select
			{},	// 2P SpeedUp
			{},	// 2P SpeedDown
		} };
	}
}
}