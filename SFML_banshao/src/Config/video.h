#pragma once
#include "general.h"

namespace game::Config
{
	class video : public config
	{
	public:
		video () {}

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(const json& j) noexcept override;
		virtual int checkValues() noexcept override;
	};
}