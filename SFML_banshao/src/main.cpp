#include "gameInstance.h"

int main()
{
	auto&& inst = game::gameInstance::getInstance();
	inst.run();
	return 0;
}