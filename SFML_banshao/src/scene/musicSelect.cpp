#include "musicSelect.h"
#include "../skin/data.h"

namespace game
{
	musicSelect::musicSelect(std::shared_ptr<Sound> pSound): Scene(pSound)
	{
		skin.init("LR2files/Theme/LR2/Select/select.lr2skin", false);
		//skin.init("LR2files/Theme/test/select.lr2skin", false);
		data().setNum(num::SEC, 222);
	}

	musicSelect::~musicSelect()
	{
	}

	void musicSelect::mainLoop()
	{
		auto rTime = data().getTimeFromStart();
		checkKeys(rTime);
	}

}