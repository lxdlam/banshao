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
	// FIXME fps
	void calc_fps_thread_func(const unsigned long long& f, sf::RenderWindow& window)
	{
		auto base = f;
		auto FrameCountInterval = std::chrono::seconds(1);
		while (window.isOpen())
		{
			base = f;
			std::this_thread::sleep_for(FrameCountInterval);
			std::cout << "frame: " << f << " fps: " << (f - base) * 1000 / std::chrono::duration_cast<std::chrono::milliseconds>(FrameCountInterval).count() << std::endl;
		}
	}

	void render_thread_func(sf::RenderWindow& window)
	{
		unsigned long long frame = 0;
		std::thread fps(calc_fps_thread_func, std::ref(frame), std::ref(window));
		std::unique_ptr<Scene> pScene(new Scene);
		while (window.isOpen())
		{
			window.clear();
			// TODO window.draw(thing); 
			window.draw(*pScene);
			window.display();
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
			std::thread render(render_thread_func, std::ref(sfWin));
			while (sfWin.isOpen())
			{
				sf::Event event;
				while (sfWin.pollEvent(event))
				{
					// "close requested" event: we close the window
					if (event.type == sf::Event::Closed)
						close();
				}
			}
			render.join();
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

}