#pragma once

#include "../Scene/scene.h"
#include <memory>
#include <string>
#include "../defs.h"

namespace game
{
	class modeController
	{
	public:
		modeController();
		modeController(std::shared_ptr<Sound>);
		~modeController();
		int start();

	private:
		using eMode = defs::eMode;
		eMode mode = eMode::UNKNOWN;
		std::shared_ptr<Sound> pSound;
		std::shared_ptr<Scene> pScene;

	public:
		eMode getCurrentMode();
		std::string getCurrentModeStr();
		int switchMode(eMode newMode);
		std::shared_ptr<Scene> getScenePtr();
	};
}