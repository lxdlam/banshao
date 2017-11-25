#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../defs.h"
#include "../sound/sound.h"
#include "../skin/skin.h"
#include <bitset>
#include <future>

namespace game
{
	using namespace defs::key;
	using namespace defs::scene;

	// Parent class of scenes, defines how an object being stored and drawn.
	// Every classes of scenes should inherit this class.
	// Must import pointer of sound system when constructing.
	// vecImage[0] & vecTexture[0] stores error texture.
	class Scene: public sf::Drawable
	{
	protected:
		//std::vector<sf::Image> vecImage;
		//std::vector<sf::Texture> vecTexture;
		//std::vector<sf::Sprite> vecSprite;
		skinClass skin;

	public:
		Scene() = delete;
		Scene(std::shared_ptr<Sound>);
		virtual ~Scene();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/*
	private:
		// Virtual function that loads textures into sprites.
		// Called at the end of constructor.
		// Should check index to prevent overflow.
		virtual void loadSprites();
	protected:
		size_t loadImage(std::string path);
		size_t loadTexture(size_t imageIdx);
		size_t loadTexture(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h);
		size_t createSprite(size_t textureidx);
		size_t createSprite(size_t imageIdx, unsigned x, unsigned y, unsigned w, unsigned h);
		*/

	private:
		bool active = false;
		bool running = false;
		bool saveCPU = true;
		int fps = 0;

		std::future<void> inputTaskFuture;
		void input_thread_func();
		std::bitset<FUNC_KEY_COUNT> prev_functionalInput = 0;
		std::bitset<GAMEPAD_KEY_COUNT> prev_gamepadInput = 0;
		std::bitset<FUNC_KEY_COUNT> functionalInput = 0;
		std::bitset<GAMEPAD_KEY_COUNT> gamepadInput = 0;
		
	public:
		void run();
		void setHighResponsive(bool);
		void setActive(bool);
		bool Scene::isFuncKeyPressed(functionalKeys k) const;
		bool Scene::isGamepadKeyPressed(gamepadKeys k) const;
		bool Scene::isFuncKeyReleased(functionalKeys k) const;
		bool Scene::isGamepadKeyReleased(gamepadKeys k) const;
		bool Scene::isFuncKeyHolding(functionalKeys k) const;
		bool Scene::isGamepadKeyHolding(gamepadKeys k) const;
		const std::bitset<FUNC_KEY_COUNT>& getFunctionalInput() const;
		const std::bitset<GAMEPAD_KEY_COUNT>& getGamepadInput() const;

	protected:
		std::shared_ptr<Sound> soundSystem;
		void checkKeys(unsigned rTime);
		virtual void mainLoop();
		void close();
	};
}
