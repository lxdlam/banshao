#pragma once
#include <SFML/Graphics.hpp>

namespace game
{
	class gameInstance
	{
	private:
		gameInstance();
		~gameInstance();

		sf::RenderWindow sfWin;
		int fps = 0;
		unsigned long long totalFrameRendered = 0;
		bool closed = false;

	public:
		static gameInstance& getInstance();
		gameInstance(gameInstance const&) = delete;
		void operator=(gameInstance const&) = delete;

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
		} mode = eMode::PLAY7;

		int run();
		int close();

	private:
		void calc_fps_thread_func();
		void render_thread_func();

	public:
		int setWindowMode();
		int setVSync();
		int setMaxFPS();
		int resize();

		inline bool isOpen();
		int getFPS();
	};
}