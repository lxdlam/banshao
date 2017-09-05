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
		bool closed = false;

	public:
		static gameInstance& getInstance();
		gameInstance(gameInstance const&) = delete;
		void operator=(gameInstance const&) = delete;

	private:
		int fps = 0;
		unsigned long long totalFrameRendered = 0;
		void calc_fps_thread_func();
		void render_thread_func();

	public:
		int run();
		int close();

		int setWindowMode();
		int setVSync();
		int setMaxFPS();
		int resize();

		inline bool isOpen();
		int getFPS();
	};
}