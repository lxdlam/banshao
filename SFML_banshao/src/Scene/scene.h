#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <future>

namespace game
{
	// Parent class of scenes, defines how an object being stored and drawn.
	// Every classes of scenes should inherit this class.
	// vecImage[0] & vecTexture[0] stores error texture.
	class Scene: public sf::Drawable
	{
	private:
		std::vector<sf::Image> vecImage;
		std::vector<sf::Texture> vecTexture;
		std::vector<sf::Sprite> vecSprite;

		// Virtual function that loads textures into sprites.
		// Called at the end of constructor.
		// Should check index to prevent overflow.
		virtual void loadSprites();

		bool running = false;
		std::future<void> inputTaskFuture;
		void input_thread_func();
		unsigned long functionalInput = 0;
		unsigned long gamepadInput = 0;

	public:
		Scene();
		~Scene();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
