#pragma once
#include <utility>

namespace game::Config
{
	class video
	{
	private:
		video() = default;
		~video() = default;
	public:
		static video& getInstance();
		video(video const&) = delete;
		void operator=(video const&) = delete;
		int setDefaults();
		int loadConfig();

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