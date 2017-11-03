#pragma once
#include "scene.h"

namespace game
{
	class musicSelect : public Scene
	{
	public:
		musicSelect() = delete;
		musicSelect(std::shared_ptr<Sound> pSound);
		~musicSelect();

	protected:
		virtual void mainLoop() override;
	};
}