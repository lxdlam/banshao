#pragma once
#include "general.h"

namespace game::Config
{
	class system : public config
	{
	public:
		system() : config("config/system.json") {}
		system(system const&) = delete;
		void operator=(system const&) = delete;

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(json& j) noexcept override;
	};
}
