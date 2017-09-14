#include "scene.h"
#include "../Input/functional.h"
#include "../Input/gamepad.h"
#include "../utils.h"
using utils::log;


namespace game
{
	Scene::Scene(std::shared_ptr<Sound> pSound)
	{
		sf::Image errImage;
		errImage.loadFromFile("resources/error.png");
		vecImage.push_back(std::move(errImage));
		sf::Texture errTexture;
		errTexture.loadFromImage(vecImage[0]);
		errTexture.setRepeated(true);
		vecTexture.push_back(std::move(errTexture));

		loadSprites();

		soundSystem = pSound;

		running = true;
		inputTaskFuture = std::async(std::launch::async, &Scene::input_thread_func, this);

		log("Scene created", LOGS_Core);
	}

	Scene::~Scene()
	{
		// Wait input thread terminates
		running = false;
		inputTaskFuture.wait();

		// Destroy sprites first (if should be done manually)

		log("Scene destroyed", LOGS_Core);
	}

	void Scene::loadSprites()
	{
		sf::Image sfImage;
		sfImage.loadFromFile("resources/placeholder.png");
		vecImage.push_back(std::move(sfImage));

		sf::Texture sfTexture;
		sfTexture.loadFromImage(vecImage[1]);
		vecTexture.push_back(std::move(sfTexture));

		vecSprite.emplace_back(vecTexture[1]);
	}

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		/// You can draw other high-level objects
		//target.draw(m_sprite, states);
		/// ... or use the low-level API
		//states.texture = &m_texture;
		//target.draw(m_vertices, states);
		/// ... or draw with OpenGL directly
		//glBegin(GL_QUADS);
		//glEnd();

		for (const auto& sprite : vecSprite)
			target.draw(sprite, states);
	}

	void Scene::input_thread_func()
	{
		using namespace std::literals::chrono_literals;
		while (running)
		{
			auto now = std::chrono::system_clock::now();

			prev_functionalInput = functionalInput;
			prev_gamepadInput = gamepadInput;
			functionalInput = active ? Input::functional::detect() : 0;
			gamepadInput = active ? Input::gamepad::detect() : 0;
			mainLoop();
			if (soundSystem) soundSystem->update();

			if (saveCPU)
				std::this_thread::sleep_until(now + 8ms);
		}
	}

	void Scene::setHighResponsive(bool b)
	{
		saveCPU = !b;
	}

	void Scene::setActive(bool a)
	{
		active = a;
	}

	bool Scene::isFuncKeyPressed(defs::functionalKeys k) const
	{
		auto m = Input::functional::mask(k);
		if ((~prev_functionalInput & m) && (functionalInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isGamepadKeyPressed(defs::gamepadKeys k) const
	{
		auto m = Input::gamepad::mask(k);
		if ((~prev_gamepadInput & m) && (gamepadInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isFuncKeyReleased(defs::functionalKeys k) const
	{
		auto m = Input::functional::mask(k);
		if ((prev_functionalInput & m) && (~functionalInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isGamepadKeyReleased(defs::gamepadKeys k) const
	{
		auto m = Input::gamepad::mask(k);
		if ((prev_gamepadInput & m) && (~gamepadInput & m))
			return true;
		else
			return false;
	}

	void Scene::mainLoop()
	{
	}

	const unsigned long& Scene::getFunctionalInput() const
	{
		return functionalInput;
	}
	const unsigned long& Scene::getGamepadInput() const
	{
		return gamepadInput;
	}

}