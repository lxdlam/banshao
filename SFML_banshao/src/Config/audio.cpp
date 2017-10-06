#include "audio.h"
#include "../defs.h"
#include <fmod_common.h>

namespace game::Config
{
	using namespace defs::config;

	void audio::setDefaults() noexcept
	{
		set<unsigned>(aud_mode, AutoDetect);
		set<unsigned>(aud_bufLen, 256);
		set<int>(aud_bufCount, 2);
	}

	int audio::copyValues(const json& j) noexcept
	{
		int c = 0;
		c += checkUnsigned(j, aud_mode);
		c += checkUnsigned(j, aud_bufLen);
		c += checkInt(j, aud_bufCount);
		return c;
	}

	int audio::checkValues() noexcept
	{
		int c = 0;
		
		// output mode
		auto mode = get<unsigned>(aud_mode);
		if (mode >= GENERAL_COUNT && mode < 10
			|| mode >= WINDOWS_COUNT && mode < 20
			|| mode >= LINUX_COUNT && mode < 30
			|| mode >= MAC_COUNT)
		{
			c++;
			set<unsigned>(aud_mode, AutoDetect);
		}

		// buffer length

		// buffer count

		return c;
	}

}