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
	private:
		gamepad() = default;
		~gamepad() = default;
		static gamepad& getInstance();

		bool haveJoystick = false;
		std::array<bool, sf::Joystick::Count> joysticksConnected{};

		using keys = game::Config::key::gamepad_keys;
		// Usage: keyboardBinds [targetKey] [bindings]
		std::array<std::vector<int>, keys::GAMEPAD_KEY_COUNT> keyboardBinds;

		// evil nesting declarations..
		// Usage: joystickBinds [joystickNo] [targetKey] [bindings]
		// Axes are added by 1000, with hundreds digit representing -/+.
		std::array<std::array<std::vector<int>, keys::GAMEPAD_KEY_COUNT>, sf::Joystick::Count> joystickBinds;

		int _deadZone = 25;

	public:
		static constexpr unsigned long mask(keys k)
		{
			return 1 << k;
		}

		static void updateBindings();
		static unsigned long detect();
	};
}
