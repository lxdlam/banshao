#include "game/gameInstance.h"

// TODO hide console window
int main()
{
	auto&& inst = game::gameInstance::getInstance();
	inst.run();
	return 0;
}