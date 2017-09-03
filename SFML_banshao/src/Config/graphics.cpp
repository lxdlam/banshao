#include "graphics.h"
#include "../utils.h"
#include <string>

using utils::log;
namespace game::Config
{
	graphics& graphics::getInstance()
	{
		static graphics _inst;
		return _inst;
	}

	int graphics::setDefaults()
	{
		setMaxFPS(-1);
		setResolution(1280, 720);
		setWindowMode(windowed::Windowed);
		setVSync(false);
		return 0;
	}

	// FIXME sleep function in SFML is based on milliseconds (at least on Windows it is)
	// which means any values higher than 1000 may not work as expected..
	int graphics::setMaxFPS(int fps)
	{
		if (fps > 0 && fps < 30)	// Not sure if minimum of 30fps is enough for a VSRG
			fps = 30;
		else if (fps <= 0)
			fps = 0;
		else if (fps > 4096)		// Hey you need too much frames!
			fps = 0;

		maxFPS = fps;
		log("[Graphics] MaxFPS -> " + std::to_string(fps), LOGS_Config);
		return 0;
	}
	decltype(graphics::maxFPS) graphics::getMaxFPS() const
	{
		return maxFPS;
	}

	int graphics::setResolution(int x, int y)
	{
		// TODO error check
		res.x = x;
		res.y = y;
		log("[Graphics] Resolution -> " + std::to_string(x) + "x" + std::to_string(y), LOGS_Config);
		return 0;
	}
	std::pair<decltype(graphics::res.x), decltype(graphics::res.y)> graphics::getResolution() const
	{
		return decltype(getResolution())({ res.x, res.y });
	}

	int graphics::setWindowMode(decltype(windowMode) w)
	{
		windowMode = w;
		log("[Graphics] Window Mode -> " + std::to_string(static_cast<int>(w)), LOGS_Config);
		return 0;
	}
	decltype(graphics::windowMode) graphics::getWindowMode() const
	{
		return windowMode;
	}

	int graphics::setVSync(bool v)
	{
		VSync= v;
		log("[Graphics] VSync -> " + std::to_string(v), LOGS_Config);
		return 0;
	}
	bool graphics::getVSync() const
	{
		return VSync;
	}
}