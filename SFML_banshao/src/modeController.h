#pragma once

#include "Scene/scene.h"
#include <memory>
#include <string>
#include <mutex>

namespace game
{
	class modeController
	{
	private:
		modeController();
		~modeController();

		enum class eMode {
			EXIT = 0,
			TITLE = 1,
			SONG_SELECT,
			THEME_SELECT,
			KEY_CONFIG,

			PLAY5,
			PLAY5_2,
			PLAY7,
			PLAY7_2,
			PLAY9,
			PLAY10,
			PLAY14,

			RESULT,
			COURSE_RESULT,

			TMPL,
		} mode;

		std::shared_ptr<Scene> pScene;

	public:
		static modeController& getInstance();
		eMode getCurrentMode();
		std::string getCurrentModeStr();
		int switchMode(eMode newMode);
		std::shared_ptr<Scene> getScenePtr();
	};
}