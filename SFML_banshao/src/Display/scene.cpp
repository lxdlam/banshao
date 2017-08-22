#include "scene.h"

namespace game::Display
{
	Scene::Scene()
	{
		//FIXME .
		a_simple_texture_initialized_from_a_file_to_be_used_by_the_sprite.loadFromFile("resources/placeholder.png");
		a_simple_random_picture_to_be_shown_on_the_window_xd_xd_xd.setTexture(a_simple_texture_initialized_from_a_file_to_be_used_by_the_sprite);
	}

	Scene::~Scene()
	{
		// Destroy sprites first (if should be done manually)
	}

	void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		/*
		// You can draw other high-level objects
		target.draw(m_sprite, states);
		// ... or use the low-level API
		states.texture = &m_texture;
		target.draw(m_vertices, states);
		// ... or draw with OpenGL directly
		glBegin(GL_QUADS);
		glEnd();
		*/

		//FIXME .
		target.draw(a_simple_random_picture_to_be_shown_on_the_window_xd_xd_xd, states);
	}
}