#include "functional.h"

#include <SFML/Window/Keyboard.hpp>

namespace game {
	namespace Input

	{
		auto functional::detect() -> std::bitset<keys::FUNC_KEY_COUNT>
		{
			using sf::Keyboard;

			std::bitset<keys::FUNC_KEY_COUNT> res{};
			if (Keyboard::isKeyPressed(Keyboard::Escape)) res[keys::ESC] = true;
			if (Keyboard::isKeyPressed(Keyboard::F1)) res[keys::F1] = true;
			if (Keyboard::isKeyPressed(Keyboard::F2)) res[keys::F2] = true;
			if (Keyboard::isKeyPressed(Keyboard::F3)) res[keys::F3] = true;
			if (Keyboard::isKeyPressed(Keyboard::F4)) res[keys::F4] = true;
			if (Keyboard::isKeyPressed(Keyboard::F5)) res[keys::F5] = true;
			if (Keyboard::isKeyPressed(Keyboard::F6)) res[keys::F6] = true;
			if (Keyboard::isKeyPressed(Keyboard::F7)) res[keys::F7] = true;
			if (Keyboard::isKeyPressed(Keyboard::F8)) res[keys::F8] = true;
			if (Keyboard::isKeyPressed(Keyboard::F9)) res[keys::F9] = true;
			if (Keyboard::isKeyPressed(Keyboard::F10)) res[keys::F10] = true;
			if (Keyboard::isKeyPressed(Keyboard::F11)) res[keys::F11] = true;
			if (Keyboard::isKeyPressed(Keyboard::F12)) res[keys::F12] = true;
			if (Keyboard::isKeyPressed(Keyboard::F13)) res[keys::F13] = true;
			if (Keyboard::isKeyPressed(Keyboard::F14)) res[keys::F14] = true;
			if (Keyboard::isKeyPressed(Keyboard::F15)) res[keys::F15] = true;
			if (Keyboard::isKeyPressed(Keyboard::Up)) res[keys::UP] = true;
			if (Keyboard::isKeyPressed(Keyboard::Down)) res[keys::DOWN] = true;
			if (Keyboard::isKeyPressed(Keyboard::Left)) res[keys::LEFT] = true;
			if (Keyboard::isKeyPressed(Keyboard::Right)) res[keys::RIGHT] = true;
			if (Keyboard::isKeyPressed(Keyboard::Return)) res[keys::RETURN] = true;
			if (Keyboard::isKeyPressed(Keyboard::BackSpace)) res[keys::BACKSPACE] = true;
			return res;
		}
	}
}