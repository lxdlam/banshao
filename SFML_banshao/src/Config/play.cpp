#include "play.h"
#include "../defs.h"

namespace game::Config
{
	using namespace defs;
	void play::setDefaults() noexcept
	{	
		set<unsigned>(pl_hs1, 100);
		set<unsigned>(pl_gr1, 1200);
		set<unsigned>(pl_sud1, 100);
		set<unsigned>(pl_hid1, 100);
		set<unsigned>(pl_lift1, 100);
		set<unsigned>(pl_mod1, 0);

		set<unsigned>(pl_hs2, 100);
		set<unsigned>(pl_gr2, 1200);
		set<unsigned>(pl_sud2, 100);
		set<unsigned>(pl_hid2, 100);
		set<unsigned>(pl_lift2, 100);
		set<unsigned>(pl_mod2, 0);
	}

	int play::copyValues(json& j) noexcept 
	{
		int c = 0;
		c += checkUnsigned(j, pl_hs1);
		c += checkUnsigned(j, pl_gr1);
		c += checkUnsigned(j, pl_sud1);
		c += checkUnsigned(j, pl_hid1);
		c += checkUnsigned(j, pl_lift1);
		c += checkUnsigned(j, pl_mod1);

		c += checkUnsigned(j, pl_hs2);
		c += checkUnsigned(j, pl_gr2);
		c += checkUnsigned(j, pl_sud2);
		c += checkUnsigned(j, pl_hid2);
		c += checkUnsigned(j, pl_lift2);
		c += checkUnsigned(j, pl_mod2);
		return c;
	}

}