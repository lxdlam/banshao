#pragma once
#include "general.h"

namespace game::Config
{
	class audio : public config
	{
	public:
		audio() {}

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(const json& j) noexcept override;
		virtual int checkValues() noexcept override;
	};
}