#pragma once
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

		enum gamepad_keys
		{
			S1L = 0,
			S1R,
			K11,
			K12,
			K13,
			K14,
			K15,
			K16,
			K17,
			K18,
			K19,
			K1START,
			K1SELECT,
			K1SPDUP,
			K1SPDDN,
			S2L,
			S2R,
			K21,
			K22,
			K23,
			K24,
			K25,
			K26,
			K27,
			K2START,
			K2SELECT,
			K2SPDUP,
			K2SPDDN,
			GAMEPAD_KEY_COUNT
		};

	private:
		int deadZone = 25;

		// For keyboard binds, the value is raw enum value defined in sf::Keyboard::Key.0
		// For joystick binds, the value is a negate number, mixing all of identifiers:
		//  10000 * Joystick No

		// Usage: gamepadKeyBindings [target] [slot] {device, code}
		//  Keyboard's device is -1
		//  code +1000 if Axis, with hundreds digit representing direction (0 for -, 1 for +)
		std::array<std::array<std::pair<int, int>, 10>, GAMEPAD_KEY_COUNT> gamepadKeyBindings = {};

		void _bindKey(gamepad_keys target, int slot, int device, int key);

	public:
		void bindKey(gamepad_keys target, int slot, sf::Keyboard::Key key);
		void bindKey(gamepad_keys target, int slot, unsigned joyNo, unsigned button);
		void bindKey(gamepad_keys target, int slot, unsigned joyNo, sf::Joystick::Axis axis, int direction);
		auto getBindings(gamepad_keys button) const -> const decltype(gamepadKeyBindings[0]);
	};
}
