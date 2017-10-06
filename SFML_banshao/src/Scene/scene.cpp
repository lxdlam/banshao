#include "scene.h"
#include "../Input/functional.h"
#include "../Input/gamepad.h"
#include "../utils.h"

namespace game
{
	Scene::Scene(std::shared_ptr<Sound> pSound)
	{
		loadImage("resources/error.png");
		vecTexture[loadTexture(0)].setRepeated(true);

		soundSystem = pSound;

		LOG(DEBUG) << "Scene created";
	}

	Scene::~Scene()
	{
		// Wait input thread terminates
		running = false;
		inputTaskFuture.wait();

		// Destroy sprites first (if should be done manually)

		LOG(DEBUG) << "Scene destroyed";
	}

	void Scene::loadSprites()
	{
		//auto imgIdx = loadImage("resources/placeholder.png");
		//auto texIdx = loadTexture(imgIdx);
		//auto sprIdx = createSprite(texIdx);
	}
	
	size_t Scene::loadImage(std::string path)
	{
		sf::Image sfImage;
		if (!sfImage.loadFromFile(path))
			LOG(WARNING) << "Load image Failed: " << path;
		vecImage.push_back(std::move(sfImage));
		return vecImage.size() - 1;
	}

	size_t Scene::loadTexture(size_t imageIdx)
	{
		sf::Texture sfTexture;
		if (!sfTexture.loadFromImage(vecImage[imageIdx]))
			LOG(WARNING) << "Load texture Failed: " << imageIdx;
		vecTexture.push_back(std::move(sfTexture));
		return vecTexture.size() - 1;
	}

	size_t Scene::loadTexture(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h)
	{
		sf::Texture sfTexture;
		if (!sfTexture.loadFromImage(vecImage[imageIdx], sf::IntRect(x, y, w, h)))
			LOG(WARNING) << "Load texture Failed: " << imageIdx;
		vecTexture.push_back(std::move(sfTexture));
		return vecTexture.size() - 1;
	}

	size_t Scene::createSprite(size_t textureIdx)
	{
		vecSprite.emplace_back(vecTexture[textureIdx]);
		return vecSprite.size() - 1;
	}

	size_t Scene::createSprite(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h)
	{
		auto textureIdx = loadTexture(imageIdx, x, y, w, h);
		return createSprite(textureIdx);
	}

	void Scene::preDraw()
	{
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
			if (running) mainLoop();
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

	void Scene::run()
	{
		running = true;
		inputTaskFuture = std::async(std::launch::async, &Scene::input_thread_func, this);
	}

	bool Scene::isFuncKeyPressed(functionalKeys k) const
	{
		return (~prev_functionalInput & mask(k)) && (functionalInput & mask(k));
	}

	bool Scene::isGamepadKeyPressed(gamepadKeys k) const
	{
		return (~prev_gamepadInput & mask(k)) && (gamepadInput & mask(k));

	}

	bool Scene::isFuncKeyReleased(functionalKeys k) const
	{
		return (prev_functionalInput & mask(k)) && (~functionalInput & mask(k));
	}

	bool Scene::isGamepadKeyReleased(gamepadKeys k) const
	{
		return (prev_gamepadInput & mask(k)) && (~gamepadInput & mask(k));
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