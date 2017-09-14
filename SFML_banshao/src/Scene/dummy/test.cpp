#include "test.h"
#include "../../Input/gamepad.h"
#include "../../utils.h"
using utils::log;

namespace game
{
	test::test(std::shared_ptr<Sound> pSound): Scene(pSound)
	{
		if (!font.loadFromFile("resources/sansation.ttf"))
			log("ERROR: Load font file failed!", LOGS_Core);
		text.setFont(font);
		text.setString("Push keys to play sounds (LS/LC, ZSXDCFV)");
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		loadSounds();

		log("test scene created", LOGS_Core);
	}

	test::~test()
	{
		log("test scene destroyed", LOGS_Core);
	}

	void test::loadSprites()
	{
	}

	void test::loadSounds()
	{
		soundSystem->loadKeySample("resources/Sound/kick_000.wav", 0);
		soundSystem->loadKeySample("resources/Sound/kick_000.wav", 1);
		soundSystem->loadKeySample("resources/Sound/cdefgab_000.wav", 2);
		soundSystem->loadKeySample("resources/Sound/cdefgab_001.wav", 3);
		soundSystem->loadKeySample("resources/Sound/cdefgab_002.wav", 4);
		soundSystem->loadKeySample("resources/Sound/cdefgab_003.wav", 5);
		soundSystem->loadKeySample("resources/Sound/cdefgab_004.wav", 6);
		soundSystem->loadKeySample("resources/Sound/cdefgab_005.wav", 7);
		soundSystem->loadKeySample("resources/Sound/cdefgab_006.wav", 8);
	}

	void test::mainLoop()
	{
		using keys = Input::gamepad::keys;
		for (size_t i = keys::S1L; i <= keys::K17; i++)
		{
			if (isGamepadKeyPressed(static_cast<keys>(i)))
				soundSystem->playKeySample(1, &i);
		}
	}

	void test::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(text, states);
	}

}