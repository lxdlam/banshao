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
			t.setPosition(0, 30.0 * idx++);
			t.setFillColor(sf::Color::White);
		}

		if (objBms.initWithFile("resources/foon_5n.bms"))
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
		notesTextureIdx = { 1, 2, 3, 2, 3, 2, 3, 2, 0, 0 };
		auto noteIdx = createSprite(noteImageIdx, 0, 0, 32, 4);
		vecSprite[noteIdx].setPosition(400, 400);
		createNoteList();
	}

	void showbms::createNoteList()
	{
		// FIXME
		auto n = objBms.getMaxMeasure();
		double time_ms = 0;
		double bpm = objBms.getBPM();
		for (unsigned m = 1; m <= n ; m++)
		{
			auto len = objBms.getMeasureLength(m);
			time_ms += 60000.0 / bpm * len;
			for (unsigned i = 0; i < 10; i++)
			{
				auto ch = objBms.getChannel(defs::bmsGetChannelCode::NOTE1, i, m);
				auto fragment = 15000.0 / bpm * len / ch.segments;
				for (const auto& note : ch.notes)
					noteLists[i].emplace_back(time_ms + fragment * note.first, note.second);
			}
		}
	}

	void showbms::createNoteVertices(std::array<sf::VertexArray, 20>& noteVertices) const
	{
		int baseX = 400;
		int baseY = 720;
		for (unsigned k = 0; k < 10; k++)
		{
			noteVertices[k].setPrimitiveType(sf::PrimitiveType::Triangles);
			int w = vecTexture[notesTextureIdx[k]].getSize().x;
			int h = vecTexture[notesTextureIdx[k]].getSize().y;
			for (const auto& note : noteLists[k])
			{
				auto y = baseY - note.first * 0.5;
				if (y < 0) continue;
				sf::Vector2f base(baseX + k * w + 5.0f, y);
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0, 0), sf::Vector2f(0, 0)));
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0), sf::Vector2f(w, 0)));
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0, h), sf::Vector2f(0, h)));
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, 0), sf::Vector2f(w, 0)));
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(0, h), sf::Vector2f(0, h)));
				noteVertices[k].append(sf::Vertex(base + sf::Vector2f(w, h), sf::Vector2f(w, h)));
			}
		}
	}

	void showbms::mainLoop()
	{

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