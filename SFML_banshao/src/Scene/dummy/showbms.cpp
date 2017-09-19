#include "showbms.h"
#include "../../Input/gamepad.h"
#include "../../utils.h"
using utils::log;

namespace game
{
	showbms::showbms(std::shared_ptr<Sound> pSound): Scene(pSound)
	{
		if (!font.loadFromFile("resources/sansation.ttf"))
			log("ERROR: Load font file failed!", LOGS_Core);
		int idx = 0;
		for (auto& t : text)
		{
			t.setFont(font);
			t.setCharacterSize(24);
			t.setPosition(0, 30.0f * idx++);
			t.setFillColor(sf::Color::White);
		}

		//if (objBms.initWithFile("resources/test.bms"))
		if (objBms.initWithFile("resources/_goodbounce_yukuri.bms"))
		{
			log("Error: load bms failed");
			text[0].setString("load failed");
			return;
		}

		auto genre = objBms.getGenre();
		auto artist = objBms.getArtist();
		auto title = objBms.getTitle();
		auto bpm = objBms.getBPM();
		auto notes = objBms.getNoteCount();
		text[0].setString(sf::String::fromUtf32(genre.begin(), genre.end()));
		text[1].setString(sf::String::fromUtf32(artist.begin(), artist.end()));
		text[2].setString(sf::String::fromUtf32(title.begin(), title.end()));
		text[3].setString(std::to_string(bpm));
		text[4].setString(std::to_string(notes));

		loadSprites();

		log("showbms scene created", LOGS_Core);
	}

	showbms::~showbms()
	{
		log("showbms scene destroyed", LOGS_Core);
	}

	void showbms::loadSprites()
	{
		auto noteImageIdx = loadImage("resources/note.png");
		loadTexture(noteImageIdx, 0, 0, 32, 4);
		loadTexture(noteImageIdx, 0, 4, 32, 4);
		loadTexture(noteImageIdx, 0, 8, 32, 4);
		notesTextureIdx = { 1, 3, 2, 3, 2, 3, 2, 3, 0, 0 };
		auto noteIdx = createSprite(noteImageIdx, 0, 0, 32, 4);
		vecSprite[noteIdx].setPosition(400, 400);
		createNoteList();
	}

	void showbms::createNoteList()
	{
		auto maxMeasure = objBms.getMaxMeasure();
		double basetime = 0;
		for (unsigned m = 0; m <= maxMeasure ; m++)
		{
			//log("MEASURE " + std::to_string(m) + " LENGTH " + std::to_string(objBms.getMeasureLength(m)), LOGS_Core);

			// notes [] {seg, {lane, sample/val}}
			std::vector<std::pair<rational, std::pair<unsigned, unsigned>>> notes;

			// Visible Notes: 0~20
			for (int i = 0; i < 10; i++)
			{
				auto ch = objBms.getChannel(defs::bmsGetChannelCode::NOTE1, i, m);
				{
					for (const auto& n : ch.notes)
					{
						notes.push_back({ rational(n.first, ch.segments), {i, n.second} });
					}
				}
			}

			// The following patterns must be arranged in specified order

			// BPM Change: 98
			{
				auto ch = objBms.getChannel(defs::bmsGetChannelCode::BPM, 0, m);
				{
					for (const auto& n : ch.notes)
						notes.push_back({ rational(n.first, ch.segments), {98, n.second} });
				}
			}

			// EX BPM: 97
			{
				auto ch = objBms.getChannel(defs::bmsGetChannelCode::EXBPM, 0, m);
				{
					for (const auto& n : ch.notes)
						notes.push_back({ rational(n.first, ch.segments), {97, n.second} });
				}
			}

			// Stop: 99
			{
				auto ch = objBms.getChannel(defs::bmsGetChannelCode::STOP, 0, m);
				{
					for (const auto& n : ch.notes)
						notes.push_back({ rational(n.first, ch.segments), {99, n.second} });
				}
			}

			// Sort by time / lane value
			std::sort(notes.begin(), notes.end());

			// Calculate note times and push to note list
			rational baseseg(0, 1);
			bool bpmfucked = false;
			double bpm = objBms.getBPM();
			double len = objBms.getMeasureLength(m) * 2.4e5;
			for (const auto& note : notes)
			{
				auto& seg = note.first;
				auto& lane = note.second.first;
				auto& val = note.second.second;
				if (lane >= 0 && lane < 20)		// Visible Notes
				{
					double timems = bpmfucked ? -1.0 : basetime + (seg - baseseg) * len / bpm;
					noteLists[lane][m].push_back({ note.first, timems, val, false });
				}
				else if (lane >= 20 && lane < 40)
				{

				}
				else if (lane >= 40 && lane < 60)
				{

				}
				else if (lane >= 60 && lane < 80)
				{

				}
				else switch (lane)
				{
				case 97:	// ExBPM Change
					basetime += (seg - baseseg) * len / bpm;
					baseseg = seg;
					bpm = objBms.getExBPM(val);
					if (bpm <= 0) bpmfucked = true;
					break;
				case 98:	// BPM Change
					basetime += (seg - baseseg) * len / bpm;
					baseseg = seg;
					bpm = static_cast<double>(val);
					if (bpm <= 0) bpmfucked = true;
					break;
				case 99:	// Stop
					double stoplen = objBms.getStop(val);
					if (stoplen <= 0) break;
					basetime += 1250.0 / bpm * stoplen;		// stoplen / 192
					break;
				}
			}
			basetime += (1.0 - baseseg) * len / bpm;
		}
	}

	void showbms::createNoteVertices(std::array<sf::VertexArray, 20>& noteVertices) const
	{
		int baseX = 400;
		int baseY = 700;
		double hispeedFactor = baseY / 700.0;
		for (unsigned m = currentMeasure; m <= objBms.getMaxMeasure(); m++)
		{
			auto mlen = objBms.getMeasureLength(m);
			for (unsigned k = 0; k < 10; k++)
			{
				noteVertices[k].setPrimitiveType(sf::PrimitiveType::Triangles);
				int w = vecTexture[notesTextureIdx[k]].getSize().x;
				int h = vecTexture[notesTextureIdx[k]].getSize().y;
				for (const auto& note : noteLists[k][m])
				{
					rational seg;
					double time;
					unsigned val;
					bool hit;
					std::tie(seg, time, val, hit) = note;
					if (hit) continue;
					float y = baseY - (seg - currentBeat) * mlen * hispeed * hispeedFactor;
					if (y < 0) break;
					sf::Vector2f base(baseX + k * w + 5.0f, y);
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0.0f), sf::Vector2f(w, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, h), sf::Vector2f(0.0f, h)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0.0f), sf::Vector2f(w, 0.0f)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0.0f, h), sf::Vector2f(0.0f, h)));
					noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, h), sf::Vector2f(w, h)));
				}
			}
			baseY -= mlen * hispeed * hispeedFactor;
			if (baseY < 0) break;
		}
	}

	void showbms::mainLoop()
	{
		if (isFuncKeyPressed(defs::functionalKeys::RIGHT))
			hispeed += 25;
		if (hispeed > 25)
			if (isFuncKeyPressed(defs::functionalKeys::LEFT))
				hispeed -= 25;

		if (currentMeasure > 0)
			if (isFuncKeyPressed(defs::functionalKeys::UP))
				currentMeasure -= 1;
		if (isFuncKeyPressed(defs::functionalKeys::DOWN))
			currentMeasure += 1;

		text[6].setString("#" + std::to_string(currentMeasure));
	}

	void showbms::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::array<sf::VertexArray, 20> noteVertices{};
		createNoteVertices(noteVertices);
		for (auto& t: text)
			target.draw(t, states);
		for (unsigned k = 0; k < 10; k++)
		{
			auto s = states;
			s.texture = &vecTexture[notesTextureIdx[k]];
			target.draw(noteVertices[k], s);
		}
	}

}