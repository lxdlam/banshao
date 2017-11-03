#pragma once
#include "../defs.h"
#include <vector>
#include <array>
#include <bitset>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Joystick.hpp>

namespace game {
	namespace Input

	{
		using namespace defs::key;

		class gamepad
		{
		public:
			using keys = gamepadKeys;

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

			void _updateBindings(unsigned K);
			std::bitset<keys::GAMEPAD_KEY_COUNT> _detect();

		public:
			static void updateBindings(unsigned K);
			static std::bitset<keys::GAMEPAD_KEY_COUNT> detect();
		};
	}
}