#include "video.h"
#include "../utils.h"
#include <string>

namespace game::Config
{
	video& video::getInstance()
	{
		static video _inst;
		return _inst;
	}

	int video::setDefaults()
	{
		setMaxFPS(0);
		setResolution(1280, 720);
		setWindowMode(windowed::Windowed);
		setVSync(false);
		return 0;
	}

	// FIXME sleep function in SFML is based on milliseconds (at least on Windows it is)
	// which means any values higher than 1000 may not work as expected..
	int video::setMaxFPS(int fps)
	{
		if (fps > 0 && fps < 30)	// Not sure if minimum of 30fps is enough for a VSRG
			fps = 30;
		else if (fps <= 0)
			fps = 0;
		else if (fps > 4096)		// Hey you need too much frames!
			fps = 0;

		maxFPS = fps;
		LOG(INFO) << "[Graphics] MaxFPS -> " << fps;
		return 0;
	}
	decltype(video::maxFPS) video::getMaxFPS() const
	{
		return maxFPS;
	}

	int video::setResolution(int x, int y)
	{
		// TODO error check
		res.x = x;
		res.y = y;
		LOG(INFO) << "[Graphics] Resolution -> " << x << "x" << y;
		return 0;
	}
	std::pair<decltype(video::res.x), decltype(video::res.y)> video::getResolution() const
	{
		return decltype(getResolution())({ res.x, res.y });
	}

	int video::setWindowMode(decltype(windowMode) w)
	{
		windowMode = w;
		LOG(INFO) << "[Graphics] Window Mode -> " << static_cast<int>(w);
		return 0;
	}
	decltype(video::windowMode) video::getWindowMode() const
	{
		return windowMode;
	}

	int video::setVSync(bool v)
	{
		VSync= v;
		LOG(INFO) << "[Graphics] VSync -> " << v;
		return 0;
	}
	bool video::getVSync() const
	{
		return VSync;
	}
}