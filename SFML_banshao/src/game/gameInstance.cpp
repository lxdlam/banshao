#include "../utils.h"
#include "../Config/configManager.h"
#include "../Scene/scene.h"
#include "../Input/functional.h"
#include "../Input/gamepad.h"
#include <SFML/Graphics.hpp>
#include "gameInstance.h"

#include <chrono>
#include <thread>

namespace game
{
	using namespace std::chrono;

	gameInstance& gameInstance::getInstance()
	{
		static gameInstance _inst;
		return _inst;
	}

	gameInstance::gameInstance()
	{
		utils::initLogging();
		LOG(INFO) << defs::name << " " << defs::subname << (defs::subname.empty() ? "" : " ")
			<< defs::versionMajor << "." << defs::versionMinor;
		configManager::getInstance();
		setWindowMode();
		Input::gamepad::updateBindings(7);
		pSound = std::make_shared<Sound>();
		modeCon = std::make_unique<modeController>(pSound);
		active = true;
		LOG(DEBUG) << "Game Instance initialized.";
	}

	gameInstance::~gameInstance()
	{
		if (isOpen())
			close();

		modeCon->switchMode(defs::eMode::EXIT);
		modeCon.reset();
		pSound.reset();
		configManager::getInstance().save();
		LOG(DEBUG) << "Game Instance destroyed.";
	}

	// FIXME I've seen an ordinary way to count FPS with a loop array
	//void gameInstance::calc_fps_thread_func()
	//{
	//	auto base = totalFrameRendered;
	//	auto FrameCountInterval = std::chrono::milliseconds(1000);
	//	while (isOpen())
	//	{
	//		base = totalFrameRendered;
	//		std::this_thread::sleep_for(FrameCountInterval);
	//		fps = static_cast<int>((totalFrameRendered - base) * 1000 / FrameCountInterval.count());
	//		//std::cout << "fps: " << fps << std::endl;
	//	}
	//}

	void gameInstance::render_thread_func()
	{
		//std::thread fps(&gameInstance::calc_fps_thread_func, this);
		while (isOpen())
		{
			sfWin.clear();
			modeCon->getScenePtr()->preDraw();
			sfWin.draw(*modeCon->getScenePtr());
			sfWin.display();
			totalFrameRendered++;
		}
		//fps.join();
	}

	int gameInstance::run()
	{
		while (isOpen())
		{
			modeCon->start();
			std::thread renderThread(&gameInstance::render_thread_func, this);
			while (sfWin.isOpen())
			{
				sf::Event event;
				while (sfWin.pollEvent(event))
				{
					switch (event.type)
					{
					case sf::Event::Closed: close(); break;
					}
				}
				modeCon->getScenePtr()->setActive(sfWin.hasFocus());
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}
			renderThread.join();
		}
		return 0;
	}

	int gameInstance::close()
	{
		if (sfWin.isOpen())
			sfWin.close();
		active = false;
		return 0;
	}

	int gameInstance::setWindowMode()
	{
		std::pair<unsigned, unsigned> resolution = { 1280, 720 };
		if (configManager::getInstance().video.get<bool>(defs::vid_FullHD))
			resolution = { 1920, 1080 };

		bool fullscreen = configManager::getInstance().video.get<bool>(defs::vid_fullscreen);
		bool borderless = configManager::getInstance().video.get<bool>(defs::vid_borderless);

		int windowStyle = 0;
		if (fullscreen)
			windowStyle = sf::Style::Fullscreen;
		else
		{
			if (borderless)
				windowStyle = sf::Style::None;
			else 
				windowStyle = sf::Style::Titlebar | sf::Style::Close;
		}

		sfWin.create(
			sf::VideoMode(resolution.first, resolution.second),
			defs::name + " " + defs::subname,
			windowStyle);
		sfWin.setActive(false);
		setMaxFPS();

		LOG(DEBUG) << "Render window set";
		return 0;
	}

	//int gameInstance::getFPS()
	//{
	//	return fps;
	//}

	bool gameInstance::isOpen()
	{
		return sfWin.isOpen() && active;
	}

	int gameInstance::setVSync()
	{
		sfWin.setVerticalSyncEnabled(configManager::getInstance().video.get<bool>(defs::vid_vsync));
		return 0;
	}

	int gameInstance::setMaxFPS()
	{
		sfWin.setFramerateLimit(configManager::getInstance().video.get<unsigned>(defs::vid_maxfps));
		return 0;
	}

}