#include "../utils.h"
#include "../Scene/scene.h"
#include "../Input/functional.h"
#include "../Input/gamepad.h"
#include <SFML/Graphics.hpp>
#include "configManager.h"
#include "gameInstance.h"

#include <chrono>
#include <thread>

namespace game
{
	using namespace std::chrono;
	using namespace defs::general;
	using namespace defs::config;

	gameInstance& gameInstance::getInstance()
	{
		static gameInstance _inst;
		return _inst;
	}

	gameInstance::gameInstance()
	{
		utils::initLogging();
		LOG(INFO) << "-----------------------------------------------------------------------";
		LOG(INFO) << name << " " << subname << (subname.empty() ? "" : " ")
			<< versionMajor << "." << versionMinor;
		config();			// initialize static singleton
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

		modeCon->switchMode(eMode::EXIT);
		modeCon.reset();
		pSound.reset();
		config().save();
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
		if (config().video.get<bool>(vid_FullHD))
			resolution = { 1920, 1080 };

		bool fullscreen = config().video.get<bool>(vid_fullscreen);
		bool borderless = config().video.get<bool>(vid_borderless);

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
			name + " " + subname,
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
		sfWin.setVerticalSyncEnabled(config().video.get<bool>(vid_vsync));
		return 0;
	}

	int gameInstance::setMaxFPS()
	{
		sfWin.setFramerateLimit(config().video.get<unsigned>(vid_maxfps));
		return 0;
	}

}