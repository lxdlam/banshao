#include "video.h"
#include "../defs.h"

namespace game {
	namespace config

	{
		using namespace defs::config;

		// FIXME sleep function in SFML is based on milliseconds (at least on Windows it is)
		// which means any values higher than 1000 may not work as expected..
		void video::setDefaults() noexcept
		{
			set<bool>(vid_FullHD, false);
			set<bool>(vid_fullscreen, false);
			set<bool>(vid_borderless, false);
			set<bool>(vid_vsync, false);
			set<unsigned>(vid_maxfps, 240);
		}

		int video::copyValues(const json& j) noexcept
		{
			int c = 0;
			c += checkBool(j, vid_FullHD);
			c += checkBool(j, vid_fullscreen);
			c += checkBool(j, vid_borderless);
			c += checkBool(j, vid_vsync);
			c += checkUnsigned(j, vid_maxfps);
			return c;
		}

		int video::checkValues() noexcept
		{
			int c = 0;

			if (get<unsigned>(vid_maxfps) > 1000)
			{
				c++;
				set<unsigned>(vid_maxfps, 240);
			}

			return c;
		}


	}

}