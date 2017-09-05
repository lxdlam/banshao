#include "functional.h"

#include <SFML/Window/Keyboard.hpp>

namespace game::Input
{
	unsigned long functional::detect()
	{
		using sf::Keyboard;

		unsigned long res = 0;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) res |= mask(ESC);
		if (Keyboard::isKeyPressed(Keyboard::F1)) res |= mask(F1);
		if (Keyboard::isKeyPressed(Keyboard::F2)) res |= mask(F2);
		if (Keyboard::isKeyPressed(Keyboard::F3)) res |= mask(F3);
		if (Keyboard::isKeyPressed(Keyboard::F4)) res |= mask(F4);
		if (Keyboard::isKeyPressed(Keyboard::F5)) res |= mask(F5);
		if (Keyboard::isKeyPressed(Keyboard::F6)) res |= mask(F6);
		if (Keyboard::isKeyPressed(Keyboard::F7)) res |= mask(F7);
		if (Keyboard::isKeyPressed(Keyboard::F8)) res |= mask(F8);
		if (Keyboard::isKeyPressed(Keyboard::F9)) res |= mask(F9);
		if (Keyboard::isKeyPressed(Keyboard::F10)) res |= mask(F10);
		if (Keyboard::isKeyPressed(Keyboard::F11)) res |= mask(F11);
		if (Keyboard::isKeyPressed(Keyboard::F12)) res |= mask(F12);
		if (Keyboard::isKeyPressed(Keyboard::F13)) res |= mask(F13);
		if (Keyboard::isKeyPressed(Keyboard::F14)) res |= mask(F14);
		if (Keyboard::isKeyPressed(Keyboard::F15)) res |= mask(F15);
		if (Keyboard::isKeyPressed(Keyboard::Up)) res |= mask(UP);
		if (Keyboard::isKeyPressed(Keyboard::Down)) res |= mask(DOWN);
		if (Keyboard::isKeyPressed(Keyboard::Left)) res |= mask(LEFT);
		if (Keyboard::isKeyPressed(Keyboard::Right)) res |= mask(RIGHT);
		if (Keyboard::isKeyPressed(Keyboard::Return)) res |= mask(RETURN);
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) res |= mask(BACKSPACE);
		return res;
	}
}