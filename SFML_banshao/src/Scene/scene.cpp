#include "scene.h"
#include "../skin/data.h"
#include "../input/functional.h"
#include "../input/gamepad.h"
#include "../utils.h"

namespace game
{
	Scene::Scene(std::shared_ptr<Sound> pSound)
	{
		//loadImage("resources/error.png");
		//vecTexture[loadTexture(0)].setRepeated(true);

		soundSystem = pSound;

		LOG(DEBUG) << "Scene created";
	}

	Scene::~Scene()
	{
		close();

		// Destroy sprites first (if should be done manually)

		LOG(DEBUG) << "Scene destroyed";
	}

	/*
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
	*/

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		skin.draw(target, states);
	}

	/*
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
	*/

	void Scene::input_thread_func()
	{
		using namespace std::literals::chrono_literals;
		while (running)
		{
			auto now = std::chrono::system_clock::now();

			prev_functionalInput = functionalInput;
			prev_gamepadInput = gamepadInput;
			functionalInput = active && skin.receiveInput() ? Input::functional::detect() : 0;
			gamepadInput = active && skin.receiveInput() ? Input::gamepad::detect() : 0;

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

	void Scene::close()
	{
		running = false;
		inputTaskFuture.wait();
	}

	bool Scene::isFuncKeyPressed(functionalKeys k) const
	{
		return (!prev_functionalInput[k]) && (functionalInput[k]);
	}

	bool Scene::isGamepadKeyPressed(gamepadKeys k) const
	{
		return (!prev_gamepadInput[k]) && (gamepadInput[k]);
	}

	bool Scene::isFuncKeyReleased(functionalKeys k) const
	{
		return (prev_functionalInput[k]) && (!functionalInput[k]);
	}

	bool Scene::isGamepadKeyReleased(gamepadKeys k) const
	{
		return (prev_gamepadInput[k]) && (!gamepadInput[k]);
	}

	bool Scene::isFuncKeyHolding(functionalKeys k) const
	{
		return (prev_functionalInput[k]) && (functionalInput[k]);
	}

	bool Scene::isGamepadKeyHolding(gamepadKeys k) const
	{
		return (prev_gamepadInput[k]) && (gamepadInput[k]);
	}

	void Scene::checkKeys(unsigned rTime)
	{
		for (size_t k = 0; k < GAMEPAD_KEY_COUNT; ++k)
		{
			if (isGamepadKeyPressed(static_cast<gamepadKeys>(k))
				&& !isGamepadKeyHolding(static_cast<gamepadKeys>(k)))
			{
				for (const auto& t : gamepadTimerPressMap[k])
					data().setTimer(t, rTime);
				for (const auto& d : gamepadDstOptPressMap[k])
					data().setDstOption(d.dstIdx, d.val);
			}
			if (isGamepadKeyReleased(static_cast<gamepadKeys>(k))
				&& !isGamepadKeyReleased(static_cast<gamepadKeys>(k)))
			{
				for (const auto& t : gamepadTimerReleaseMap[k])
					data().setTimer(t, rTime);
				for (const auto& d : gamepadDstOptReleaseMap[k])
					data().setDstOption(d.dstIdx, d.val);
			}
		}
	}

	void Scene::mainLoop()
	{
		auto rTime = data().getTimeFromStart();
		checkKeys(rTime);
	}

	const std::bitset<FUNC_KEY_COUNT>& Scene::getFunctionalInput() const
	{
		return functionalInput;
	}

	const std::bitset<GAMEPAD_KEY_COUNT>& Scene::getGamepadInput() const
	{
		return gamepadInput;
	}


}