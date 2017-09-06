#include "audio.h"

namespace game::Config
{
	audio& audio::getInstance()
	{
		static audio _inst;
		return _inst;
	}

	int audio::setDefaults()
	{
		bufferLength = 256;
		bufferCount = 2;
		return 0;
	}

	unsigned audio::getBufferLength()
	{
		return bufferLength;
	}

	int audio::getBufferCount()
	{
		return bufferCount;
	}
}
