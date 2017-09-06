#include "test.h"
#include "../../Input/gamepad.h"
#include "../../utils.h"
#include "../../Sound/sound.h"
using utils::log;

namespace game
{
	test::test()
	{
		if (!font.loadFromFile("resources/sansation.ttf"))
			log("ERROR: Load font file failed!", LOGS_Core);
		text.setFont(font);
		text.setString("Push keys to play sounds (LS/LC, ZSXDCFV)");
		text.setCharacterSize(24);
		text.setFillColor(sf::Color::White);

		loadSounds();
	}

	test::~test()
	{
	}

	void test::loadSprites()
	{
	}

	void test::loadSounds()
	{
		Sound::loadKeySample("resources/Sound/kick_000.wav", FMOD_DEFAULT, 0, 0);
		Sound::loadKeySample("resources/Sound/kick_000.wav", FMOD_DEFAULT, 0, 1);
		Sound::loadKeySample("resources/Sound/cdefgab_000.wav", FMOD_DEFAULT, 0, 2);
		Sound::loadKeySample("resources/Sound/cdefgab_001.wav", FMOD_DEFAULT, 0, 3);
		Sound::loadKeySample("resources/Sound/cdefgab_002.wav", FMOD_DEFAULT, 0, 4);
		Sound::loadKeySample("resources/Sound/cdefgab_003.wav", FMOD_DEFAULT, 0, 5);
		Sound::loadKeySample("resources/Sound/cdefgab_004.wav", FMOD_DEFAULT, 0, 6);
		Sound::loadKeySample("resources/Sound/cdefgab_005.wav", FMOD_DEFAULT, 0, 7);
		Sound::loadKeySample("resources/Sound/cdefgab_006.wav", FMOD_DEFAULT, 0, 8);
	}

	void test::logic()
	{
		using keys = Input::gamepad::keys;
		for (size_t i = keys::S1L; i <= keys::K17; i++)
		{
			if (isGamepadKeyPressed(static_cast<keys>(i)))
				Sound::playKeySample(1, &i);
		}
		Sound::update();
	}

	void test::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(text, states);
	}

}