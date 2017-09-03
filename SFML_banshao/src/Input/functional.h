#pragma once

namespace game::Input
{
	class functional
	{
	public:
		functional() = delete;
		~functional() = delete;

		static enum functional_keys
		{
			ESC = 0,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			UP,
			DOWN,
			LEFT,
			RIGHT,
			RETURN,
			BACKSPACE,
			FUNC_KEY_COUNT
		};

		static constexpr unsigned long mask(functional_keys k)
		{
			return 1 << k;
		}

		static unsigned long detect();
	};
}
