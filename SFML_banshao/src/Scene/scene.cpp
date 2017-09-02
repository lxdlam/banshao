#include "scene.h"

namespace game
{
	Scene::Scene()
	{
		sf::Image sfImage;
		sfImage.loadFromFile("resources/placeholder.png");
		vecImage.push_back(std::move(sfImage));

		sf::Texture sfTexture;
		sfTexture.loadFromImage(vecImage[0]);
		vecTexture.push_back(std::move(sfTexture));
		for (const auto& texture : vecTexture)
			vecSprite.emplace_back(texture);
	}

	Scene::~Scene()
	{
		// Destroy sprites first (if should be done manually)
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
}