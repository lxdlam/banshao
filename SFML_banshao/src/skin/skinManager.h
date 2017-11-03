#pragma once
#include <array>
#include <memory>
#include "../defs.h"
#include "../utils.h"
#include "skin.h"


namespace game
{
	using defs::general::eMode;
	using namespace utils;
	typedef std::shared_ptr<skinClass> pSkin;

	class skinManager
	{
	private:
		skinManager() = default;
		~skinManager() = default;
	public:
		skinManager(skinManager&) = delete;
		skinManager& operator= (skinManager&) = delete;
		friend skinManager& skin();

	protected:
		std::array<pSkin, static_cast<size_t>(eMode::MODE_COUNT)> inst{};

	public:
		void load(eMode);
		pSkin get(eMode);
		void reset(eMode);
	};

	skinManager& skin();
}