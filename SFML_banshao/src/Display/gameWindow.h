#pragma once
#include <SFML/Graphics.hpp>

namespace game::Display
{
	class window
	{
	private:
		sf::RenderWindow sfWin;
		int fps;
		bool closed = false;

	public:
		window();
		~window();

		int run();
		int close();

		int setWindowMode();
		int setVSync();
		int setMaxFPS();
		int resize();

		inline bool isOpen();

	};




}