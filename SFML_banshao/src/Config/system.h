#pragma once
#include "configBase.h"

namespace game {
	namespace config

	{
		class system : public configBase
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
}