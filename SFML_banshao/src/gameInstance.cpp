#include "utils.h"
#include "Config/general.h"
#include "Config/graphics.h"
#include "Config/system.h"
#include "Scene/scene.h"
#include "Input/functional.h"
#include "Input/gamepad.h"
#include <SFML/Graphics.hpp>
#include "gameInstance.h"
#include "modeController.h"

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
		// TODO init with file
		Config::init();
		utils::logSystemInfo();
		closed = false;
		setWindowMode();
		Input::gamepad::updateBindings();
	}

	gameInstance::~gameInstance()
	{
		if (isOpen())
			close();

		log("-------------------------------------------------------------");

		Config::saveConfig();
		Config::dispose();
	}

	// FIXME I've seen an ordinary way to count FPS with a loop array
	void gameInstance::calc_fps_thread_func()
	{
		auto base = totalFrameRendered;
		auto FrameCountInterval = std::chrono::milliseconds(1000);
		while (isOpen())
		{
			base = totalFrameRendered;
			std::this_thread::sleep_for(FrameCountInterval);
			fps = static_cast<int>((totalFrameRendered - base) * 1000 / FrameCountInterval.count());
			std::cout << "fps: " << fps << std::endl;
		}
	}

	void gameInstance::render_thread_func()
	{
		std::thread fps(&gameInstance::calc_fps_thread_func, this);
		auto& modeCon = game::modeController::getInstance();
		while (sfWin.isOpen())
		{
			sfWin.clear();
			sfWin.draw(*modeCon.getScenePtr());
			sfWin.display();
			totalFrameRendered++;
		}
		fps.join();
	}

	int gameInstance::run()
	{
		while (!closed)
		{
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
		closed = true;
		return 0;
	}

	int gameInstance::setWindowMode()
	{
		using Config::graphics;
		auto& graphicInst = graphics::getInstance();
		auto resolution = graphicInst.getResolution();

		int windowStyle = 0;
		switch (graphicInst.getWindowMode())
		{
		case graphics::windowed::Borderless:
			windowStyle = sf::Style::None; break;
		case graphics::windowed::Fullscreen:
			windowStyle = sf::Style::Fullscreen; break;
		case graphics::windowed::Windowed:
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

	int gameInstance::getFPS()
	{
		return fps;
	}

	bool gameInstance::isOpen()
	{
		return sfWin.isOpen() || !closed;
	}

	int gameInstance::setVSync()
	{
		sfWin.setVerticalSyncEnabled(Config::graphics::getInstance().getVSync());
		return 0;
	}

	int gameInstance::setMaxFPS()
	{
		sfWin.setFramerateLimit(Config::graphics::getInstance().getMaxFPS());
		return 0;
	}

	int gameInstance::resize()
	{
		auto resolution = Config::graphics::getInstance().getResolution();
		sfWin.setSize(sf::Vector2u(resolution.first, resolution.second));
		return 0;
	}
}