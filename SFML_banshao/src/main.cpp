#include "game/gameInstance.h"

#if !_DEBUG && _WIN32
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	auto&& inst = game::gameInstance::getInstance();
	inst.run();
	return 0;
}