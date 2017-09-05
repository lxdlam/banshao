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

	void key::_bindKey(gamepad_keys target, int slot, int device, int key)
	{
		gamepadKeyBindings[target][slot] = {device, key};
	}

	void key::bindKey(gamepad_keys target, int slot, sf::Keyboard::Key key)
	{
		_bindKey(target, slot, -1, static_cast<int>(key));
	}

	void key::bindKey(gamepad_keys target, int slot, unsigned joyNo, unsigned button)
	{
		_bindKey(target, slot, static_cast<int>(joyNo), static_cast<int>(button));
	}

	void key::bindKey(gamepad_keys target, int slot, unsigned joyNo, sf::Joystick::Axis axis, int direction)
	{
		bindKey(target, slot, joyNo, 1000 + (direction > 0 ? 100 : 0) + axis);
		
	}

	auto key::getBindings(gamepad_keys k) const -> const decltype(gamepadKeyBindings[0])
	{
		return gamepadKeyBindings[k];
	}
}