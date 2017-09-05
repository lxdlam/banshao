#include "modeController.h"
#include <string>
#include <mutex>

namespace game
{
	modeController& modeController::getInstance()
	{
		static modeController _inst;
		return _inst;
	}

	modeController::modeController()
	{
		switchMode(eMode::UNKNOWN);
	}

	modeController::~modeController() {}
	
	modeController::eMode modeController::getCurrentMode()
	{
		return mode;
	}

	std::string modeController::getCurrentModeStr()
	{
		switch (mode)
		{
		case eMode::EXIT:			return "Exit";
		case eMode::TITLE:			return "Title";
		case eMode::SONG_SELECT:	return "Song Select";
		case eMode::THEME_SELECT:	return "Theme Select";
		case eMode::KEY_CONFIG:		return "Key Config";

		case eMode::PLAY5:			return "5Keys";
		case eMode::PLAY5_2:		return "7Keys Battle";
		case eMode::PLAY7:			return "7Keys";
		case eMode::PLAY7_2:		return "7Keys Battle";
		case eMode::PLAY9:			return "9Keys";
		case eMode::PLAY10:			return "5Keys DP";
		case eMode::PLAY14:			return "7Keys DP";

		case eMode::RESULT:			return "Result";
		case eMode::COURSE_RESULT:	return "Course Result";

		default:					return "Template";
		}
	}

	int modeController::switchMode(eMode newMode)
	{
		mode = newMode;
		// also change pScene here
		switch (mode)
		{
		/*
		case eMode::EXIT: 			break;
		case eMode::TITLE: 			break;
		case eMode::SONG_SELECT: 	break;
		case eMode::THEME_SELECT: 	break;
		case eMode::KEY_CONFIG: 	break;

		case eMode::PLAY5: 			break;
		case eMode::PLAY5_2: 		break;
		case eMode::PLAY7: 			break;
		case eMode::PLAY7_2: 		break;
		case eMode::PLAY9: 			break;
		case eMode::PLAY10: 		break;
		case eMode::PLAY14: 		break;

		case eMode::RESULT: 		break;
		case eMode::COURSE_RESULT: 	break;
		*/
		default:					pScene = std::make_shared<Scene>(); break;
		}
		return 0;
	}


	std::shared_ptr<Scene> modeController::getScenePtr()
	{
		return pScene;
	}
}