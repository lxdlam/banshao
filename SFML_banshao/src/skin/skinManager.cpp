#include "skinManager.h"
#include <filesystem>

namespace game
{
	skinManager& skin()
	{
		static skinManager _inst;
		return _inst;
	}

	void skinManager::load(eMode e)
	{
		auto& inst = this->inst[static_cast<size_t>(e)];
		if (inst != nullptr)
			reset(e);
	}

	pSkin skinManager::get(eMode e)
	{
		auto& inst = this->inst[static_cast<size_t>(e)];
		if (inst == nullptr)
			load(e);
		return inst;
	}
	void skinManager::reset(eMode e)
	{
		inst[static_cast<size_t>(e)].reset();
	}
}