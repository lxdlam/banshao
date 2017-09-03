#include "key.h"
#include <SFML/Window/Keyboard.hpp>

#include "../utils.h"
using utils::log;

namespace game::Config
{
	key& key::getInstance()
	{
		static key _inst;
		return _inst;
	}

	int key::setDefaults()
	{
		deadZone = 25;
		bindKey(gamepad_keys::S1L, 0, sf::Keyboard::LShift);
		bindKey(gamepad_keys::S1R, 0, sf::Keyboard::LControl);
		bindKey(gamepad_keys::K11, 0, sf::Keyboard::Z);
		bindKey(gamepad_keys::K12, 0, sf::Keyboard::X);
		bindKey(gamepad_keys::K13, 0, sf::Keyboard::C);
		bindKey(gamepad_keys::K14, 0, sf::Keyboard::V);
		bindKey(gamepad_keys::K15, 0, sf::Keyboard::B);
		bindKey(gamepad_keys::K16, 0, sf::Keyboard::N);
		bindKey(gamepad_keys::K17, 0, sf::Keyboard::M);
		return 0;
	}

	int key::loadConfig()
	{
		// TODO 
		return 1;
	}

	void key::bindKey(gamepad_keys target, int index, int key)
	{
		gamepadKeyBindings[target][index] = key;
	}

	void key::bindKey(gamepad_keys target, int index, sf::Keyboard::Key key)
	{
		bindKey(target, index, static_cast<int>(key));
	}

	void key::bindKey(gamepad_keys target, int index, unsigned joyNo, unsigned button)
	{
		bindKey(target, index, -static_cast<int>(joyNo * 10000 + button));
	}

	void key::bindKey(gamepad_keys target, int index, unsigned joyNo, sf::Joystick::Axis axis, int direction)
	{
		if (direction > 0) direction = 1;
		else if (direction < 0) direction = 0;
		bindKey(target, index, -static_cast<int>(joyNo * 10000 + 1000 + direction * 100 + static_cast<int>(axis)));
	}

	auto key::getBindings(gamepad_keys k) const -> const decltype(gamepadKeyBindings[0])
	{
		return gamepadKeyBindings[k];
	}
}