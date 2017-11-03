#pragma once

#include "../scene/scene.h"
#include <memory>
#include <string>
#include "../defs.h"

namespace game
{
	using namespace defs::general;

	class modeController
	{
	public:
		modeController();
		modeController(std::shared_ptr<Sound>);
		~modeController();
		int start();

	private:
		eMode mode;
		std::shared_ptr<Sound> pSound;
		std::shared_ptr<Scene> pScene;

	public:
		eMode getCurrentMode();
		std::string getCurrentModeStr();
		int switchMode(eMode newMode);
		std::shared_ptr<Scene> getScenePtr();
	};
}