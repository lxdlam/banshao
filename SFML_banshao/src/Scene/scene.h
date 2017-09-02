#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace game
{
	// Parent class of scenes, defines how an object being stored and drawn.
	// Every sub classes of scenes should inherit this class.
	class Scene: public sf::Drawable
	{
	private:
		// TODO �涨Ԫ�����ͣ���δ洢�����Ѱ�Ҳ����ƣ�
		// FIXME .
		sf::Texture a_simple_texture_initialized_from_a_file_to_be_used_by_the_sprite;
		sf::Sprite a_simple_random_picture_to_be_shown_on_the_window_xd_xd_xd;
	public:
		Scene();
		~Scene();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
