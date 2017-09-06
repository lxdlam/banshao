#pragma once

#include <SFML/Graphics.hpp>
#include <fmod.hpp>
#include <array>
#include "../scene.h"

namespace game
{
	class test : public Scene
	{
	public:
		test();
		~test();
	private:
		sf::Font font;
		sf::Text text;
		void loadSprites() override;
		void loadSounds();
	protected:
		void logic() override;
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
