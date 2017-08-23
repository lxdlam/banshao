#include "gameWindow.h"
#include "../utils.h"
#include "../Config/graphics.h"
#include "../Config/system.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <thread>
#include <memory>

#include "scene.h"

using utils::log;

namespace game::Display
{
	void window::calc_fps_thread_func(const unsigned long long& f)
	{
		auto base = f;
		auto FrameCountInterval = std::chrono::seconds(1);
		while (isOpen())
		{
			base = f;
			std::this_thread::sleep_for(FrameCountInterval);
			fps = (f - base) * 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(FrameCountInterval).count();
			std::cout << "frame: " << f << " fps: " << fps << std::endl;
		}
	}

	void window::render_thread_func()
	{
		unsigned long long frame = 0;
		std::thread fps(&window::calc_fps_thread_func, this, std::ref(frame));
		std::unique_ptr<Scene> pScene(new Scene);
		while (sfWin.isOpen())
		{
			sfWin.clear();
			// TODO sfWin.draw(thing); 
			sfWin.draw(*pScene);
			sfWin.display();
			frame++;
		}
		fps.join();
	}

	window::window()
	{
		closed = false;
	}

	window::~window()
	{
		if (!closed)
			close();
	}

	int window::run()
	{
		while (!closed)
		{
			std::thread renderThread(&window::render_thread_func, this);
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
					// FIXME How long should event checking takes?
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				}
			}
			renderThread.join();
		}
		return 0;
	}

	int window::close()
	{
		if (sfWin.isOpen())
			sfWin.close();
		closed = true;
		return 0;
	}

	int window::setWindowMode()
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

	int window::setVSync()
	{
		sfWin.setVerticalSyncEnabled(Config::graphics::getInstance().getVSync());
		return 0;
	}

	int window::setMaxFPS()
	{
		sfWin.setFramerateLimit(Config::graphics::getInstance().getMaxFPS());
		return 0;
	}

	int window::resize()
	{
		auto resolution = Config::graphics::getInstance().getResolution();
		sfWin.setSize(sf::Vector2u(resolution.first, resolution.second));
		return 0;
	}

	bool window::isOpen()
	{
		return sfWin.isOpen() || !closed;
	}

	int window::getFPS()
	{
		return fps;
	}

}