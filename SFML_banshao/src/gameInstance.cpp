#include "utils.h"
#include "Config/general.h"
#include "Config/video.h"
#include "Config/system.h"
#include "Scene/scene.h"
#include "Input/functional.h"
#include "Input/gamepad.h"
#include <SFML/Graphics.hpp>
#include "gameInstance.h"
#include "Sound/sound.h"

#include <chrono>
#include <thread>

using utils::log;

namespace game
{
	gameInstance& gameInstance::getInstance()
	{
		static gameInstance _inst;
		return _inst;
	}

	gameInstance::gameInstance()
	{
		Config::init();
		utils::logSystemInfo();
		setWindowMode();
		Input::gamepad::updateBindings();
		pSound = std::make_shared<Sound>();
		modeCon = std::make_unique<modeController>(pSound);
		active = true;
		log("Game Instance initialized.", LOGS_Core);
	}

	gameInstance::~gameInstance()
	{
		if (isOpen())
			close();

		modeCon->switchMode(defs::eMode::EXIT);
		modeCon.reset();
		pSound.reset();
		Config::saveConfig();
		log("Game Instance destroyed.", LOGS_Core);
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
					// "close requested" event: we close the window
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
		using Config::video;
		auto& graphicInst = video::getInstance();
		auto resolution = graphicInst.getResolution();

		int windowStyle = 0;
		switch (graphicInst.getWindowMode())
		{
		case video::windowed::Borderless:
			windowStyle = sf::Style::None; break;
		case video::windowed::Fullscreen:
			windowStyle = sf::Style::Fullscreen; break;
		case video::windowed::Windowed:
			windowStyle = sf::Style::Titlebar | sf::Style::Close; break;
		}
		sfWin.create(
			sf::VideoMode(resolution.first, resolution.second),
			Config::system::getInstance().getTitle(),
			windowStyle);
		sfWin.setActive(false);
		setMaxFPS();

		log("Render window set", LOGS_Core);
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
		sfWin.setVerticalSyncEnabled(Config::video::getInstance().getVSync());
		return 0;
	}

	int gameInstance::setMaxFPS()
	{
		sfWin.setFramerateLimit(Config::video::getInstance().getMaxFPS());
		return 0;
	}

	int gameInstance::resize()
	{
		auto resolution = Config::video::getInstance().getResolution();
		sfWin.setSize(sf::Vector2u(resolution.first, resolution.second));
		return 0;
	}
}