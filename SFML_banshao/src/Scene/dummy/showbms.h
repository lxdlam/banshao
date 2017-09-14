#pragma once
#include <SFML/Graphics.hpp>
#include <fmod.hpp>
#include <array>
#include "../../Sound/sound.h"
#include "../../bms/bms.h"
#include "../scene.h"

namespace game
{
	class showbms : public Scene
	{
	public:
		showbms() = delete;
		showbms(std::shared_ptr<Sound> pSound);
		~showbms();
	private:
		bms objBms;
		sf::Font font;
		sf::Text text[20];
		void loadSprites() override;
	protected:
		void mainLoop() override;
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
