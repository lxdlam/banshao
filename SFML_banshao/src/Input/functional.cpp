#include "functional.h"

#include <SFML/Window/Keyboard.hpp>

namespace game::Input
{
	unsigned long functional::detect()
	{
		using sf::Keyboard;

		unsigned long res = 0;
		if (Keyboard::isKeyPressed(Keyboard::Escape)) res |= mask(keys::ESC);
		if (Keyboard::isKeyPressed(Keyboard::F1)) res |= mask(keys::F1);
		if (Keyboard::isKeyPressed(Keyboard::F2)) res |= mask(keys::F2);
		if (Keyboard::isKeyPressed(Keyboard::F3)) res |= mask(keys::F3);
		if (Keyboard::isKeyPressed(Keyboard::F4)) res |= mask(keys::F4);
		if (Keyboard::isKeyPressed(Keyboard::F5)) res |= mask(keys::F5);
		if (Keyboard::isKeyPressed(Keyboard::F6)) res |= mask(keys::F6);
		if (Keyboard::isKeyPressed(Keyboard::F7)) res |= mask(keys::F7);
		if (Keyboard::isKeyPressed(Keyboard::F8)) res |= mask(keys::F8);
		if (Keyboard::isKeyPressed(Keyboard::F9)) res |= mask(keys::F9);
		if (Keyboard::isKeyPressed(Keyboard::F10)) res |= mask(keys::F10);
		if (Keyboard::isKeyPressed(Keyboard::F11)) res |= mask(keys::F11);
		if (Keyboard::isKeyPressed(Keyboard::F12)) res |= mask(keys::F12);
		if (Keyboard::isKeyPressed(Keyboard::F13)) res |= mask(keys::F13);
		if (Keyboard::isKeyPressed(Keyboard::F14)) res |= mask(keys::F14);
		if (Keyboard::isKeyPressed(Keyboard::F15)) res |= mask(keys::F15);
		if (Keyboard::isKeyPressed(Keyboard::Up)) res |= mask(keys::UP);
		if (Keyboard::isKeyPressed(Keyboard::Down)) res |= mask(keys::DOWN);
		if (Keyboard::isKeyPressed(Keyboard::Left)) res |= mask(keys::LEFT);
		if (Keyboard::isKeyPressed(Keyboard::Right)) res |= mask(keys::RIGHT);
		if (Keyboard::isKeyPressed(Keyboard::Return)) res |= mask(keys::RETURN);
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) res |= mask(keys::BACKSPACE);
		return res;
	}
}