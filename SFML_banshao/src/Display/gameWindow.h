#pragma once
#include <SFML/Graphics.hpp>

namespace game::Display
{
	class window
	{
	private:
		sf::RenderWindow sfWin;
		int fps = 0;
		bool closed = false;

	public:
		window();
		~window();

		void calc_fps_thread_func(const unsigned long long& f);
		void render_thread_func();

		int run();
		int close();

		int setWindowMode();
		int setVSync();
		int setMaxFPS();
		int resize();

		inline bool isOpen();
		inline int getFPS();
	};




}