#pragma once
#include "general.h"

namespace game::Config
{
	class play : public config
	{
	public:
		play() : config("config/play.json") {}
		play(play const&) = delete;
		void operator=(play const&) = delete;

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(json& j) noexcept override;
	};
}
