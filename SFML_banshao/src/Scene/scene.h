#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "../defs.h"
#include "../Sound/sound.h"
#include <future>

namespace game
{
	using namespace defs::key;

	// Parent class of scenes, defines how an object being stored and drawn.
	// Every classes of scenes should inherit this class.
	// Must import pointer of sound system when constructing.
	// vecImage[0] & vecTexture[0] stores error texture.
	class Scene: public sf::Drawable
	{
	protected:
		std::vector<sf::Image> vecImage;
		std::vector<sf::Texture> vecTexture;
		std::vector<sf::Sprite> vecSprite;

	public:
		Scene() = delete;
		Scene(std::shared_ptr<Sound>);
		~Scene();
		virtual void preDraw();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

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

	private:
		bool active = false;
		bool running = false;
		bool saveCPU = true;
		int fps = 0;
		std::future<void> inputTaskFuture;
		void input_thread_func();
		unsigned long prev_functionalInput = 0;
		unsigned long prev_gamepadInput = 0;
		unsigned long functionalInput = 0;
		unsigned long gamepadInput = 0;
		
	public:
		void run();
		void setHighResponsive(bool);
		void setActive(bool);
		bool Scene::isFuncKeyPressed(functionalKeys k) const;
		bool Scene::isGamepadKeyPressed(gamepadKeys k) const;
		bool Scene::isFuncKeyReleased(functionalKeys k) const;
		bool Scene::isGamepadKeyReleased(gamepadKeys k) const;
		const unsigned long& getFunctionalInput() const;
		const unsigned long& getGamepadInput() const;

	protected:
		std::shared_ptr<Sound> soundSystem;
		virtual void mainLoop();
	};
}
