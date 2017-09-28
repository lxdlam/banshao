#include "configManager.h"

namespace game
{
	configManager::configManager()
	{
		load();
	}

	configManager::~configManager()
	{
		save();
	}

	configManager& configManager::getInstance()
	{
		static configManager _inst;
		return _inst;
	}

	void configManager::load()
	{
		audio.loadFile();
		video.loadFile();
		play.loadFile();
		key.loadFile();
		system.loadFile();
	}

	void configManager::save()
	{
		audio.saveFile();
		video.saveFile();
		play.saveFile();
		key.saveFile();
		system.saveFile();
	}
}
