#pragma once
#include "general.h"

namespace game::Config
{
	class system : public config
	{
	public:
		system() {}

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(const json& j) noexcept override;
		virtual int checkValues() noexcept override;
	};
}
