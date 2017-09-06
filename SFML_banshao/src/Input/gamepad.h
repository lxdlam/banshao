#pragma once
#include "../Config/key.h"
#include <vector>
#include <array>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

namespace game::Input
{
	class gamepad
	{
	public:
		using keys = game::Config::key::gamepad_keys;

	private:
		gamepad() = default;
		~gamepad() = default;
		static gamepad& getInstance();

		bool haveJoystick = false;
		std::array<bool, sf::Joystick::Count> joysticksConnected{};

		// Usage: keyboardBinds [targetKey] [bindings]
		std::array<std::vector<int>, keys::GAMEPAD_KEY_COUNT> keyboardBinds;

		// evil nesting declarations..
		// Usage: joystickBinds [joystickNo] [targetKey] [bindings]
		//  code +1000 if Axis, with hundreds digit representing direction (0 for -, 1 for +)
		std::array<std::array<std::vector<int>, keys::GAMEPAD_KEY_COUNT>, sf::Joystick::Count> joystickBinds;

		int deadZone = 25;

		void _updateBindings();
		unsigned long _detect();

	public:
		static constexpr unsigned long mask(keys k)
		{
			return 1 << k;
		}

		static void updateBindings();
		static unsigned long detect();
	};
}
