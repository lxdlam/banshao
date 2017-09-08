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

		// loadConfig(default);
		bindKey(keys::S1L, 0, sf::Keyboard::LShift);
		bindKey(keys::S1R, 0, sf::Keyboard::LControl);
		bindKey(keys::K11, 0, sf::Keyboard::Z);
		bindKey(keys::K12, 0, sf::Keyboard::S);
		bindKey(keys::K13, 0, sf::Keyboard::X);
		bindKey(keys::K14, 0, sf::Keyboard::D);
		bindKey(keys::K15, 0, sf::Keyboard::C);
		bindKey(keys::K16, 0, sf::Keyboard::F);
		bindKey(keys::K17, 0, sf::Keyboard::V);
		return 0;
	}

	void key::_bindKey(keys target, int slot, int device, int key)
	{
		gamepadKeyBindings[target][slot] = {device, key};
	}

	void key::bindKey(keys target, int slot, sf::Keyboard::Key key)
	{
		_bindKey(target, slot, -1, static_cast<int>(key));
	}

	void key::bindKey(keys target, int slot, unsigned joyNo, unsigned button)
	{
		_bindKey(target, slot, static_cast<int>(joyNo), static_cast<int>(button));
	}

	void key::bindKey(keys target, int slot, unsigned joyNo, sf::Joystick::Axis axis, int direction)
	{
		bindKey(target, slot, joyNo, 1000 + (direction > 0 ? 100 : 0) + axis);
		
	}

	auto key::getBindings(keys k) const -> const decltype(gamepadKeyBindings[0])
	{
		return gamepadKeyBindings[k];
	}
}