#include "audio.h"
#include "../defs.h"
#include "../utils.h"
#include <string>
#include <fmod_common.h>

namespace game::Config
{
	using namespace defs;
	void audio::setDefaults() noexcept
	{
		set<unsigned>(aud_mode, AutoDetect);
		set<unsigned>(aud_bufLen, 256);
		set<int>(aud_bufCount, 2);
	}

	int audio::copyValues(json& j) noexcept
	{
		int c = 0;
		c += checkUnsigned(j, aud_mode);
		c += checkUnsigned(j, aud_bufLen);
		c += checkInt(j, aud_bufCount);
		return c;
	}

}