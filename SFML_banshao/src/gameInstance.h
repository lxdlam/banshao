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

	private:
		int input_fps = 0;
		unsigned long long totalInputDetected = 0;
		unsigned long functionalInput = 0;
		unsigned long gamepadInput = 0;
		void calc_input_fps_thread_func();
		void input_thread_func();
	public:
		inline const unsigned long& getFunctionalInput() const;
		inline const unsigned long& getGamepadInput() const;

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