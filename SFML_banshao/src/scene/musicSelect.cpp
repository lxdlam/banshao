#include "musicSelect.h"
#include "../skin/data.h"

namespace game
{
	musicSelect::musicSelect(std::shared_ptr<Sound> pSound): Scene(pSound)
	{
		skin.init("LR2files/Theme/LR2/Select/select.lr2skin", false);
	}

	musicSelect::~musicSelect()
	{
	}

	void musicSelect::mainLoop()
	{
		auto rTime = data().getTimeFromStart();
		checkKeys(rTime);
		if (skin.receiveInput())
		{
			if (!startedInput)
			{
				// things to do the time starting to receive input
				startedInput = true;
			}

		}
	}

}