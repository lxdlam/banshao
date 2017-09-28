#pragma once
#include "general.h"

namespace game::Config
{
	class audio : public config
	{
	public:
		audio() : config("config/audio.json") {}
		audio(audio const&) = delete;
		void operator=(audio const&) = delete;

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(json& j) noexcept override;
	};
}