#pragma once
#include <SFML/Graphics.hpp>
#include <fmod.hpp>
#include <chrono>
#include <array>
#include "../../Sound/sound.h"
#include "../../bms/bms.h"
#include "../scene.h"
#include "../../types.hpp"

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
		// noteLists [key] [measure] {beat, time(ms), sample/value, hit}}
		std::array<std::array<std::list<std::tuple<rational, double, int, bool>>, defs::MAXMEASUREIDX + 1>, 20> noteLists;
		std::array<std::list<std::tuple<rational, double, int, bool>>, defs::MAXMEASUREIDX + 1> bpmLists;
		std::array<std::list<std::tuple<rational, double, int, bool>>, defs::MAXMEASUREIDX + 1> stopLists;
		void createNoteList();
		std::array<size_t, 20> notesTextureIdx{};
		void createNoteVertices(std::array<sf::VertexArray, 20>&) const;

		// controllable variables
		int hispeed = 100;

		// state variables
		unsigned currentMeasure = 0;
		double currentBeat = 0;
		bool started = false;
		std::chrono::system_clock startTime;

	protected:
		void mainLoop() override;
	public:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
