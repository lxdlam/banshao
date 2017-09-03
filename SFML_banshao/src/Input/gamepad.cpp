#include "gamepad.h"
#include "../utils.h"
using utils::log;

namespace game::Input
{
	gamepad& gamepad::getInstance()
	{
		static gamepad _inst;
		return _inst;
	}

	void gamepad::updateBindings()
	{
		auto& kbd = getInstance().keyboardBinds;
		auto& joy = getInstance().joystickBinds;
		auto& joys = getInstance().joysticksConnected;
		auto& haveJoy = getInstance().haveJoystick;

		for (auto& k : kbd)
			k.clear();
		for (auto& joysticks : joy)
			for (auto& k : joysticks)
				k.clear();
		haveJoy = false;

		for (unsigned i = 0; i < sf::Joystick::Count; i++)
		{
			joys[i] = sf::Joystick::isConnected(i);
			if (joys[i]) haveJoy = true;
		}

		for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
		{
			auto eKey = static_cast<keys>(k);
			auto binding = game::Config::key::getInstance().getBindings(eKey);
			for (auto b : binding)
			{
				if (b > 0)
				{
					//Keyboard
					kbd[eKey].push_back(b);
				}
				else if (b < 0)
				{
					//Joysticks
					int joyNo = -b / 10000;

					joy[joyNo][eKey].push_back(-b % 10000);
				}
			}
		}

		log("Key bindings updated", LOGS_Core);
	}

	unsigned long gamepad::detect()
	{
		const auto& kbd = getInstance().keyboardBinds;

		using sf::Keyboard;
		unsigned long res = 0;
		for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
		{
			for (auto b: kbd[k])
				if (Keyboard::isKeyPressed(static_cast<Keyboard::Key>(b)))
				{
					res |= mask(static_cast<keys>(k));
					break;
				}
		}

		if (getInstance().haveJoystick)
		{
			using sf::Joystick;
			const auto& joys = getInstance().joysticksConnected;
			const auto& joy = getInstance().joystickBinds;
			const auto& deadzone = getInstance()._deadZone;
			for (int k = keys::S1L; k < keys::GAMEPAD_KEY_COUNT; k++)
			{
				if (res & mask(static_cast<keys>(k))) continue;
				for (int i = 0; i < joys.size(); i++)
				{
					if (!joys[i]) continue;
					if (res & mask(static_cast<keys>(k))) break;
					for (auto b : joy[i][k])
					{
						if (b / 1000)
						{
							b = b % 1000;
							//Axis
							int axis = b % 100;
							int direction = b / 100;
							auto pos = Joystick::getAxisPosition(i, static_cast<Joystick::Axis>(axis));
							if (direction > 0)
							{
								if (pos - deadzone > 0)
								{
									res |= mask(static_cast<keys>(k));
									break;
								}
							}
							else
							{
								if (pos + deadzone < 0)
								{
									res |= mask(static_cast<keys>(k));
									break;
								}
							}
						}
						else
						{
							//Button
							b = b % 1000;
							if (Joystick::isButtonPressed(i, b))
							{
								res |= mask(static_cast<keys>(k));
								break;
							}
						}
					}
				}
			}
		}

		return res;
	}
}