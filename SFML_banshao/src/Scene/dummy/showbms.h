#pragma once
#include <SFML/Graphics.hpp>
#include <fmod.hpp>
#include <chrono>
#include <queue>
#include <array>
#include "../../Sound/sound.h"
#include "../../bms/bms.h"
#include "../scene.h"
#include "../../defs.h"
#include "../../types.hpp"
using namespace game::defs::bms;
using namespace game::defs::judge;

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
		typedef std::tuple<rational, double, unsigned, bool> note;
		typedef std::tuple<rational, double, unsigned> subNote;
		// noteLists [key] [measure] {beat, time(ms), sample/value, hit}}
		std::array<std::array<std::list<note>, MAXMEASUREIDX + 1>, 20> noteLists;
		std::array<std::array<std::list<note>, defs::bms::MAXMEASUREIDX + 1>, BGMCHANNELS> bgmLists;
		std::array<std::list<subNote>, MAXMEASUREIDX + 1> bpmLists;
		std::array<std::list<subNote>, MAXMEASUREIDX + 1> stopLists;
		std::array<double, MAXMEASUREIDX + 1> measureTimeList;
		void createNoteList();
		std::array<size_t, 20> notesTextureIdx{};
		void createNoteVertices(std::array<sf::VertexArray, 20>&) const;

		// control variables
		int hispeed = 100;
		std::array<unsigned, 10> keySample{};
		judge_t judgeTime{};
		judgeArea judgeAreaCheck(double noteTime, double time);
		std::pair<judgeArea, double> judgeNote(note& note, double time);

		// state variables
		unsigned drawMeasure = 0;
		double drawSeg = 0;
		double drawBPM = 0;
		double drawBasetime = 0;
		rational drawBaseseg = rational(0, 1);
		decltype(bpmLists[0].begin()) itBPM;
		decltype(stopLists[0].begin()) itStop;
		std::pair<rational, double> drawStopUntil;

		bool started = false;
		std::chrono::time_point<std::chrono::system_clock> startTime;

	protected:
		void mainLoop() override;
		void start();
	public:
		virtual void preDraw() override;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	};
}
