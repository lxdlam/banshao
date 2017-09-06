#include "scene.h"


namespace game
{
	Scene::Scene()
	{
		sf::Image errImage;
		errImage.loadFromFile("resources/error.png");
		vecImage.push_back(std::move(errImage));
		sf::Texture errTexture;
		errTexture.loadFromImage(vecImage[0]);
		errTexture.setRepeated(true);
		vecTexture.push_back(std::move(errTexture));

		loadSprites();

		running = true;
		inputTaskFuture = std::async(std::launch::async, &Scene::input_thread_func, this);
	}

	Scene::~Scene()
	{
		// Wait input thread terminates
		running = false;
		inputTaskFuture.wait();

		// Destroy sprites first (if should be done manually)
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
		while (running)
		{
			prev_functionalInput = functionalInput;
			prev_gamepadInput = gamepadInput;
			functionalInput = Input::functional::detect();
			gamepadInput = Input::gamepad::detect();
			logic();
			// FIXME We are not burning our CPUs!
		}
	}

	bool Scene::isFuncKeyPressed(Input::functional::functional_keys k) const
	{
		auto m = Input::functional::mask(k);
		if ((~prev_functionalInput & m) && (functionalInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isGamepadKeyPressed(Input::gamepad::keys k) const
	{
		auto m = Input::gamepad::mask(k);
		if ((~prev_gamepadInput & m) && (gamepadInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isFuncKeyReleased(Input::functional::functional_keys k) const
	{
		auto m = Input::functional::mask(k);
		if ((prev_functionalInput & m) && (~functionalInput & m))
			return true;
		else
			return false;
	}

	bool Scene::isGamepadKeyReleased(Input::gamepad::keys k) const
	{
		auto m = Input::gamepad::mask(k);
		if ((prev_gamepadInput & m) && (~gamepadInput & m))
			return true;
		else
			return false;
	}

	void Scene::logic()
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