#pragma once
#include <SFML/Graphics.hpp>
#include "Sound/sound.h"
#include "modeController.h"

namespace game
{
	class gameInstance
	{
	private:
		gameInstance();
		~gameInstance();

		sf::RenderWindow sfWin;
		std::shared_ptr<Sound> pSound;
		std::unique_ptr<modeController> modeCon;
		bool active = false;

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