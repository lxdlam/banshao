#pragma once

#include <memory>
#include "Display/gameWindow.h"

namespace game
{
	class gameInstance
	{
	private:
		gameInstance();
		~gameInstance();
		Display::window window;

	public:
		static gameInstance& getInstance();
		gameInstance(gameInstance const&) = delete;
		void operator=(gameInstance const&) = delete;

		// FIXME Maybe this should not be here
		enum class eMode {
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
		};

		int run();
		int getWindowFPS();
	};
}