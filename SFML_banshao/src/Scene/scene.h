#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace game
{
	// Parent class of scenes, defines how an object being stored and drawn.
	// Every classes of scenes should inherit this class.
	class Scene: public sf::Drawable
	{
	private:
		std::vector<sf::Image> vecImage;
		std::vector<sf::Texture> vecTexture;
		std::vector<sf::Sprite> vecSprite;
	public:
		Scene();
		~Scene();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
