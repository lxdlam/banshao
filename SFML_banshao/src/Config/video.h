#pragma once
#include "general.h"

namespace game::Config
{
	class video : public config
	{
	public:
		video() : config("config/video.json") {}
		video(video const&) = delete;
		void operator=(video const&) = delete;

	public:
		virtual void setDefaults() noexcept override;
	protected:
		virtual int copyValues(json& j) noexcept override;
	};
}