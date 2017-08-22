#pragma once
#include <utility>

namespace game::Config
{
	class graphics
	{
	private:
		graphics() = default;
		~graphics() = default;
	public:
		static graphics& getInstance();
		graphics(graphics const&) = delete;
		void operator=(graphics const&) = delete;

	public:
		enum class windowed {
			Fullscreen = 0,
			Windowed,
			Borderless
		};
		struct resolution {
				int x;
				int y;
			};

	private:
		unsigned maxFPS;
		resolution res;
		windowed windowMode;
		bool VSync;

	public:

		int setDefaults();

		int setMaxFPS(int fps);
		unsigned getMaxFPS() const;

		int setResolution(int x, int y);
		std::pair<decltype(res.x), decltype(res.y)> getResolution() const;

		int setWindowMode(decltype(windowMode));
		decltype(windowMode) getWindowMode() const;

		int setVSync(bool);
		bool getVSync() const;
	};
}