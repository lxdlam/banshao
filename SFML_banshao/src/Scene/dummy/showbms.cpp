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
			t.setPosition(0, 30 * idx++);
			t.setFillColor(sf::Color::White);
		}

		objBms.initWithFile("resources/foon_5n.bms");
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

		log("showbms scene created", LOGS_Core);
	}

	showbms::~showbms()
	{
		log("showbms scene destroyed", LOGS_Core);
	}

	void showbms::loadSprites()
	{
	}

	void showbms::mainLoop()
	{

	}

	void showbms::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto& t: text)
			target.draw(t, states);
	}

}