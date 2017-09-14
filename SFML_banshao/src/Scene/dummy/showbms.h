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
		// resources
		sf::Font font;
		sf::Text text[20];
		void loadSprites() override;

		// variables
		bms objBms;
		std::array<std::list<std::pair<double, int>>, 20> noteLists;	// noteLists [key] [time(ms)] [sample/value]
		void createNoteList();
		std::array<size_t, 20> notesTextureIdx{};
		void createNoteVertices(std::array<sf::VertexArray, 20>&) const;

		// controllable variables
		int hispeed = 100;

	protected:
		void mainLoop() override;
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
