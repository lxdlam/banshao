#include "gamepad.h"
#include "../game/configManager.h"
#include "../utils.h"

namespace game {
	namespace Input

	{
		using namespace defs::config;

		gamepad& gamepad::getInstance()
		{
			static gamepad _inst;
			return _inst;
		}

		void gamepad::_updateBindings(unsigned K)
		{
			for (auto& k : keyboardBinds)
				k.clear();
			for (auto& joysticks : joystickBinds)
				for (auto& k : joysticks)
					k.clear();

			haveJoystick = false;
			for (unsigned i = 0; i < sf::Joystick::Count; i++)
				if (sf::Joystick::isConnected(i))
				{
					joysticksConnected[i] = true;
					haveJoystick = true;
				}

			for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
			{
				auto eKey = static_cast<keys>(k);
				auto binding = conf().key.getBindings(K, eKey);
				for (auto b : binding)
				{
					if (b.first == -1)
						keyboardBinds[eKey].push_back(b.second);
					else
						joystickBinds[b.first][eKey].push_back(b.second);
				}
			}

			LOG(INFO) << "Key bindings updated";
		}

		auto gamepad::_detect() -> std::bitset<keys::GAMEPAD_KEY_COUNT>
		{
			std::bitset<keys::GAMEPAD_KEY_COUNT> res{};

			using sf::Keyboard;
			for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
			{
				for (auto b : keyboardBinds[k])
				{
					if (b > 1000) continue;
					if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(b)))
					{
						res[k] = true;
						break;
					}
				}
			}

			// Skip joystick detect if no joysticks connected
			if (!haveJoystick)
				return res;

			using sf::Joystick;
			for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
			{
				for (int i = 0; i < sf::Joystick::Count; i++)
				{
					if (res[k]) break;
					if (!joysticksConnected[i]) continue;
					for (auto b : joystickBinds[i][k])
					{
						if (b / 1000)
						{
							//Axis
							b = b % 1000;
							int axis = b % 100;
							int direction = b / 100 == 0 ? -1 : 1;
							auto pos = Joystick::getAxisPosition(i, static_cast<Joystick::Axis>(axis));
							if (direction * pos - deadZone > 0)
								res[k] = true;;
						}
						else
						{
							//Button
							b = b % 1000;
							if (Joystick::isButtonPressed(i, b))
								res[k] = true;
						}
					}
				}
			}

			return res;
		}

		void gamepad::updateBindings(unsigned K)
		{
			getInstance()._updateBindings(K);
		}

		auto gamepad::detect() -> std::bitset<keys::GAMEPAD_KEY_COUNT>
		{
			return getInstance()._detect();
		}
	}
}