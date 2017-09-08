#pragma once
#include "../defs.h"
#include <array>
#include <utility>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

namespace game::Config
{
	class key
	{
	private:
		key() = default;
		~key() = default;
	public:
		static key& getInstance();
		key(key const&) = delete;
		void operator=(key const&) = delete;
		int setDefaults();
		int loadConfig();

		using keys = defs::gamepadKeys;

	private:
		int deadZone = 25;

		// For keyboard binds, the value is raw enum value defined in sf::Keyboard::Key.0
		// For joystick binds, the value is a negate number, mixing all of identifiers:
		//  10000 * Joystick No

		// Usage: gamepadKeyBindings [target] [slot] {device, code}
		//  Keyboard's device is -1
		//  code +1000 if Axis, with hundreds digit representing direction (0 for -, 1 for +)
		std::array<std::array<std::pair<int, int>, 10>, keys::GAMEPAD_KEY_COUNT> gamepadKeyBindings = {};

		void _bindKey(keys target, int slot, int device, int key);

	public:
		void bindKey(keys target, int slot, sf::Keyboard::Key key);
		void bindKey(keys target, int slot, unsigned joyNo, unsigned button);
		void bindKey(keys target, int slot, unsigned joyNo, sf::Joystick::Axis axis, int direction);
		auto getBindings(keys button) const -> const decltype(gamepadKeyBindings[0]);
	};
}
