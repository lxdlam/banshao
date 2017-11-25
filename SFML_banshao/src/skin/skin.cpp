#include "skin.h"
#include "data.h"
#include "../defs.h"
#include "../utils.h"
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

namespace game
{
	skinClass::skinClass()
	{
		sf::Image err;
		err.loadFromFile(defs::file::errorTextureImage);
		vecImage.push_back(std::move(err));

		imgBlackDot.create(1, 1, sf::Color::Black);
		imgWhiteDot.create(1, 1, sf::Color::White);
		texStageFile.create(640, 480);
		texBackBmp.create(256, 256);
		texBanner.create(300, 80);
		texSkinThumbnail.create(640, 480);
		texBlackDot.loadFromImage(imgBlackDot);
		texWhiteDot.loadFromImage(imgWhiteDot);
	}

	skinClass::skinClass(path path, bool extended) : skinClass()
	{
		init(path, extended);
	}

	void skinClass::init(path path, bool extended)
	{
		if (!extended)
			loadLR2Skin(path);
		// TODO Extended skin format ?

		//data().setTimer(timer::INPUT_START, startInputTime);
		freeImages();
	}
	
#pragma region LR2 csv parsing

	std::vector<std::string> skinClass::csvNextLineTokenize(std::istream& file)
	{
		++line;
		std::string lineRaw;
		std::getline(file, lineRaw);

		if (lineRaw.empty() || lineRaw.substr(0, 2) == "//")
			return {};

		while (lineRaw.length() > 0 && lineRaw[lineRaw.length() - 1] == '\r')
			lineRaw.pop_back();

		if (lineRaw.empty())
			return {};

		// replace "\" with "\\"
		std::istringstream iss(lineRaw);
		std::string buf, line;
		while (std::getline(iss, buf, '\\'))
			line += buf + R"(\\)";
		line.erase(line.length() - 2);

		std::vector<std::string> ret;
		auto tokens = tokenizer(line, boost::escaped_list_separator<char>());
		for (auto& t : tokens) ret.push_back(t);
		while (!ret.empty() && ret.back().empty()) ret.pop_back();
		return ret;
	}


	int skinClass::loadLR2image(const std::vector<std::string> &t)
	{
		using std::string;
		auto it = t.begin();
		if (*it == "#IMAGE")
		{
			path path(*++it);
			if (path.stem() == "*")
			{
				auto ls = utils::findFiles(path);
				if (!ls.empty())
				{
					unsigned idx = std::rand() % ls.size();
					imagePath.push_back(ls[idx]);
				}
				else
				{
					imagePath.push_back(defs::file::errorTextureImage);
				}
				return 2;
			}
			else
				imagePath.push_back(std::move(path));
			return 1;
		}
		return 0;
	}

	int skinClass::loadLR2font(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		if (*it == "#LR2FONT")
		{
			path path(*++it);
			lr2fontPath.push_back(std::move(path));
			return 1;
		}
		return 0;
	}

	int skinClass::loadLR2systemfont(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		if (*it == "#FONT")
		{
			// TODO
		}
		return 0;
	}

	int skinClass::loadLR2include(const std::vector<std::string> &t)
	{
		using std::string;
		auto it = t.begin();
		if (*it == "#INCLUDE")
		{
			path path(*++it);
			auto line = this->line;
			this->line = 0;
			loadLR2Skin(path);
			this->line = line;
			return 1;
		}
		return 0;
	}

	int skinClass::loadLR2timeoption(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		if (*it == "#STARTINPUT")
		{
			int start = std::stoi(*++it);
			if (start > 0) startInputTime = start;
			if (mode == eMode::RESULT || mode == eMode::COURSE_RESULT)
			{
				int rank = std::stoi(*++it);
				int update = std::stoi(*++it);
				if (rank > 0) resultStartInputTimeRank = rank;
				if (update > 0) resultStartInputTimeUpdate = update;
			}

			return 1;
		}

		else if (*it == "#SKIP")
		{
			int time = std::stoi(*++it);
			if (time > 0) startSkipTime = time;
			return 2;
		}

		else if (*it == "#LOADSTART")
		{
			int time = std::stoi(*++it);
			if (time > 0) loadStartTime = time;
			return 3;
		}

		else if (*it == "#LOADEND")
		{
			int time = std::stoi(*++it);
			if (time > 0) loadTimeLengthThreshold = time;
			return 4;
		}

		else if (*it == "#PLAYSTART")
		{
			int time = std::stoi(*++it);
			if (time > 0) playStartAfterLoad = time;
			return 5;
		}

		else if (*it == "#CLOSE")
		{
			int time = std::stoi(*++it);
			if (time > 0) stageFailedTimeLength = time;
			return 6;
		}

		else if (*it == "#FADEOUT")
		{
			int time = std::stoi(*++it);
			if (time > 0) fadeoutLength = time;
			return 7;
		}

		return 0;
	}

	int skinClass::loadLR2others(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		if (*it == "#RELOADBANNER")
		{
			reloadBanner = true;
			return 1;
		}
		if (*it == "#TRANSCOLOR")
		{
			int r, g, b;
			r = g = b = 0;
			r = std::stoi(*++it);
			g = std::stoi(*++it);
			b = std::stoi(*++it);
			if (r < 0) r = 0;
			if (g < 0) g = 0;
			if (b < 0) b = 0;
			transColor = {
				static_cast<unsigned>(r),
				static_cast<unsigned>(g),
				static_cast<unsigned>(b)
			};
			return 2;
		}
		if (*it == "#FLIPSIDE")
		{
			flipSide = true;
			return 3;
		}
		if (*it == "#FLIPRESULT")
		{
			flipResult = true;
			return 4;
		}
		if (*it == "#DISABLEFLIP")
		{
			disableFlipResult = true;
			return 5;
		}
		if (*it == "#SCRATCH")
		{
			int a, b;
			a = !!std::stoi(*++it);
			b = !!std::stoi(*++it);
			scratchSide1P = a;
			scratchSide2P = b;
			return 6;
		}
		return 0;
	}
	
	int skinClass::loadLR2src(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		auto opt = *it;

		if (opt == "#SRC_TEXT")
		{
			if (t.size() < 5)
			{
				LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
				return 0;
			}

			int font, st, align, edit, panel;
			font = st = align = edit = panel = 0;
			++it;
			font = std::stoi(*++it);
			st = std::stoi(*++it);
			align = std::stoi(*++it);
			if (++it != t.end())
			{
				edit = std::stoi(*it);
				if (++it != t.end())
					panel = std::stoi(*it);
			}

			elements.emplace_back();
			auto& e = elements.back();
			e = std::make_shared<elemText>();

			return 7;
		}

		if (opt != "#SRC_IMAGE" && opt != "#SRC_NUMBER" &&
			opt != "#SRC_SLIDER" && opt != "#SRC_BARGRAPH" &&
			opt != "#SRC_BUTTON" && opt != "#SRC_ONMOUSE")
			return 0;

		if (t.size() < 11)
		{
			LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
			return 0;
		}

		++it;
		int gr = std::stoi(*++it) + 1;			// 0th is used by ERROR
		int x = std::stoi(*++it);
		int y = std::stoi(*++it);
		int w = std::stoi(*++it);
		int h = std::stoi(*++it);
		int div_x = std::stoi(*++it);
		int div_y = std::stoi(*++it);
		int cycle = std::stoi(*++it);
		int timer = std::stoi(*++it);

		if (div_x == 0)
		{
			LOG(WARNING) << "[Skin] div_x is 0 (Line " << line << ")";
			div_x = 1;
		}
		if (div_y == 0)
		{
			LOG(WARNING) << "[Skin] div_y is 0 (Line " << line << ")";
			div_y = 1;
		}

		int ret = 0;
		elements.emplace_back();
		auto& e = elements.back();

		if (opt == "#SRC_IMAGE")
		{
			e = std::make_shared<element>(gr, x, y, w, h, div_x, div_y, cycle, timer);
			ret = 1;
		}
		else if (opt == "#SRC_NUMBER")
		{
			int num, align, keta;
			if (t.size() < 14)
			{
				LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
				num = align = keta = 0;
			}
			else
			{
				num = std::stoi(*++it);
				align = std::stoi(*++it);
				keta = std::stoi(*++it);
			}

			e = std::make_shared<elemNumber>(gr, x, y, w, h, div_x, div_y, cycle, timer, num, align, keta);
			auto numPtr = std::dynamic_pointer_cast<elemNumber>(e);
			if (e->type() == elementType::NUMBER && numPtr)
			{
				numPtr->num = num;
				numPtr->alignType = align;
				numPtr->digits = keta;
			}
			else
			{
				LOG(WARNING) << "[Skin] src / dst Type mismatch (Line " << line << ")";
			}

			ret = 2;
		}
		else if (opt == "#SRC_SLIDER")
		{
			int muki, range, type, disable;
			if (t.size() < 14)
			{
				LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
				muki = range = type = disable = 0;
			}
			else
			{
				muki = std::stoi(*++it);
				range = std::stoi(*++it);
				type = std::stoi(*++it);
				disable = 0;
				if (++it != t.end())
					disable = std::stoi(*++it);
			}

			e = std::make_shared<elemSlider>();

			ret = 3;
		}
		else if (opt == "#SRC_BARGRAPH")
		{
			e = std::make_shared<elemBargraph>();
			ret = 4;
		}
		else if (opt == "#SRC_BUTTON")
		{
			e = std::make_shared<elemButton>();
			ret = 5;
		}
		else if (opt == "#SRC_ONMOUSE")
		{
			e = std::make_shared<elemOnMouse>();
			ret = 6;
		}
		return ret;
	}
	
	int skinClass::loadLR2dst(const std::vector<std::string> &t)
	{
		auto it = t.begin();
		auto opt = *it;

		if (*it != "#DST_IMAGE" && *it != "#DST_NUMBER" && *it != "#DST_SLIDER" &&
			*it != "#DST_BARGRAPH" && *it != "#DST_BUTTON" && *it != "#DST_ONMOUSE" && 
			*it != "#DST_TEXT" || elements.empty())
			return 0;

		if (t.size() < 14)
		{
			LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
			return 0;
		}

		++it;
		int time = std::stoi(*++it);
		int x = std::stoi(*++it);
		int y = std::stoi(*++it);
		int w = std::stoi(*++it);
		int h = std::stoi(*++it);
		int acc = std::stoi(*++it);
		int a = std::stoi(*++it);
		int r = std::stoi(*++it);
		int g = std::stoi(*++it);
		int b = std::stoi(*++it);
		int blend = std::stoi(*++it);
		int filter = std::stoi(*++it);
		int angle, center;
		angle = center = 0;
		if (++it != t.end())
		{
			angle = std::stoi(*it);
			if (++it != t.end())
			{
				center = std::stoi(*it);
			}
		}
		int ret = 0;
		auto e = elements.back();
		if (e == nullptr)
		{
			LOG(WARNING) << "[Skin] Previous src definition invalid (Line: " << line << ")";
			return 0;
		}

		if (opt == "#DST_IMAGE")
		{
			ret = 1;
		}
		else if (opt == "#DST_NUMBER")
		{
			ret = 2;
		}
		else if (opt == "#DST_BARGRAPH")
		{
			ret = 3;
		}
		else if (opt == "#DST_BUTTON")
		{
			ret = 4;
		}
		else if (opt == "#DST_ONMOUSE")
		{
			ret = 5;
		}

		if (e->empty())
		{
			int loop, dstTimer, op1, op2, op3, op4;
			if (t.size() < 18)
			{
				LOG(WARNING) << "[Skin] Parameter not enough (Line " << line << ")";
				loop = -1;
				dstTimer = op1 = op2 = op3 = op4 = 0;
			}
			else
			{
				loop = std::stoi(*++it);
				dstTimer = std::stoi(*++it);
				op1 = op2 = op3 = op4 = 0;
				if (++it != t.end())
				{
					op1 = std::stoi(*it);
					if (++it != t.end())
					{
						op2 = std::stoi(*it);
						if (++it != t.end())
						{
							op3 = std::stoi(*it);
							if (++it != t.end())
								op4 = std::stoi(*it);
						}
					}
				}
			}

			e->setDstParam(loop, dstTimer, op1, op2, op3, op4);
		}

		e->pushKeyFrame(time, x, y, w, h, acc, r, g, b, a, blend, filter, angle, center);

		return ret;
	}

	int skinClass::loadLR2SkinLine(const std::vector<std::string> &t)
	{
		try {
			if (loadLR2image(t))
				return 1;
			if (loadLR2font(t))
				return 2;
			if (loadLR2systemfont(t))
				return 3;
			if (loadLR2include(t))
				return 4;
			if (loadLR2timeoption(t))
				return 5;
			if (loadLR2others(t))
				return 6;
			if (loadLR2src(t))
				return 7;
			if (loadLR2dst(t))
				return 8;
		}
		catch (std::invalid_argument e)
		{
			LOG(WARNING) << "[Skin] Invalid Argument: " << "(Line " << line << ")";
		}
		catch (std::out_of_range e)
		{
			LOG(WARNING) << "[Skin] Out of range: " << "(Line " << line << ")";
		}
		return 0;
	}

	void skinClass::loadLR2IF(const std::vector<std::string> &t, std::ifstream& lr2skin)
	{
		bool optSwitch = true;
		if (++t.begin() == t.end() && *t.begin() != "#ELSE")
		{
			LOG(WARNING) << "[Skin] No IF parameters " << " (Line " << line << ")";
		}
		for (auto it = ++t.begin(); it != t.end(); ++it)
		{
			auto opt = utils::stoub(*it);
			if (opt.first == -1)
			{
				LOG(WARNING) << "[Skin] Invalid DST_OPTION Index, deal as false (Line" << line << ")";
				optSwitch = false;
				break;
			}
			bool dstoption = data().getDstOption(static_cast<dst_option>(opt.first));
			if (opt.second) dstoption = !dstoption;
			optSwitch = optSwitch && dstoption;
		}

		if (optSwitch)
		{
			while (!lr2skin.eof())
			{
				auto tokens = csvNextLineTokenize(lr2skin);
				if (tokens.begin() == tokens.end()) continue;

				if (*tokens.begin() == "#ELSE" || *tokens.begin() == "#ELIF")
				{
					while (!lr2skin.eof() && *tokens.begin() != "#ENDIF")
					{
						tokens = csvNextLineTokenize(lr2skin);
						if (tokens.begin() == tokens.end()) continue;
					}
					return;
				}
				else if (*tokens.begin() == "#ENDIF")
					return;
				else
					loadLR2SkinLine(tokens);
			}
		}
		else
		{
			while (!lr2skin.eof())
			{
				auto tokens = csvNextLineTokenize(lr2skin);
				if (tokens.begin() == tokens.end()) continue;

				if (*tokens.begin() == "#ELSE")
				{
					loadLR2IF(tokens, lr2skin);
					return;
				}
				else if (*tokens.begin() == "#ELSEIF")
				{
					loadLR2IF(tokens, lr2skin);
					return;
				}
				else if (*tokens.begin() == "#ENDIF")
					return;
			}
		}
	}

	int skinClass::loadLR2header(const std::vector<std::string> &t)
	{
		using std::string;
		auto it = t.begin();
		if (*it == "#INFORMATION")
		{
			int type = std::stoi(*++it);
			string title = *++it;
			string maker = *++it;
			path thumbnail(*++it);

			switch (type)
			{
			case 0:		mode = eMode::PLAY7;	break;
			case 1:		mode = eMode::PLAY5;	break;
			case 2:		mode = eMode::PLAY14;	break;
			case 3:		mode = eMode::PLAY10;	break;
			case 4:		mode = eMode::PLAY9;	break;
			case 5:		mode = eMode::MUSIC_SELECT;	break;
			case 6:		mode = eMode::DECIDE;	break;
			case 7:		mode = eMode::RESULT;	break;
			case 8:		mode = eMode::KEY_CONFIG;	break;
			case 9:		mode = eMode::THEME_SELECT;	break;
			case 10:	mode = eMode::SOUNDSET;	break;
			case 12:	mode = eMode::PLAY5_2;	break;
			case 13:	mode = eMode::PLAY7_2;	break;
			case 15:	mode = eMode::COURSE_RESULT;	break;

			case 17:	mode = eMode::TITLE;	break;
			case 16:	mode = eMode::PLAY9_2;	break;
				// 11: THEME
				// 14: COURSE EDIT
				// 18: MODE SELECT
				// 19: MODE DECIDE
				// 20: COURSE SELECT
			}
			this->title = title;
			this->maker = maker;
			if (!imgSkinThumbnail.loadFromFile(std::string(thumbnail.string())))
				LOG(WARNING) << "[Skin] thumbnail loading failed: " << thumbnail << " (Line " << line << ")";

			texSkinThumbnail.loadFromImage(imgSkinThumbnail);
			return 1;
		}

		else if (*it == "#CUSTOMOPTION")
		{
			string title = *++it;
			int dst_op = std::stoi(*++it);
			if (dst_op < 900 || dst_op > 999)
			{
				LOG(WARNING) << "[Skin] Invalid option value: " << dst_op << " (Line " << line << ")";
				return -2;
			}
			std::vector<string> op_label;
			while (++it != t.end() && !it->empty())
				op_label.push_back(*it);

			customize.emplace_back(dst_op, title, std::move(op_label), 0);
			return 2;
		}

		else if (*it == "#CUSTOMFILE")
		{
			string title = *++it;
			string tmp = *++it;
			path pathf(tmp);
			path def(*++it);

			auto ls = utils::findFiles(pathf);
			unsigned defVal = 0;
			for (size_t param = 0; param < ls.size(); ++param)
				if (ls[param] == def)
				{
					defVal = param;
					break;
				}

			customFile.emplace_back(title, std::move(ls), defVal, defVal);
			return 3;
		}

		else if (*it == "#ENDOFHEADER")
		{
			return -1;
		}

		return 0;
	}

#pragma endregion

	void skinClass::loadLR2Skin(path path)
	{
		std::ifstream lr2skin(path, std::ios::binary);
		if (!lr2skin.is_open())
		{
			LOG(ERROR) << "[Skin] Skin File Not Found: " << path;
			return;
		}

		while (!lr2skin.eof())
		{
			auto tokens = csvNextLineTokenize(lr2skin);
			if (tokens.begin() == tokens.end()) continue;
			
			if (loadLR2header(tokens) == -1)
				break;
		}

		if (lr2skin.eof())
		{
			lr2skin.close();
			lr2skin.open(path, std::ios::binary);
		}

		while (!lr2skin.eof())
		{
			auto tokens = csvNextLineTokenize(lr2skin);
			if (tokens.begin() == tokens.end()) continue;

			if (*tokens.begin() == "#IF")
				loadLR2IF(tokens, lr2skin);
			else
				loadLR2SkinLine(tokens);
		}
	

		LOG(DEBUG) << "[Skin] Loaded " << path;

		loadImages();
		convertImageToTexture();
		for (auto& e : elements)
		{
			if (e && e->type() != elementType::TEXT)
			{
				createSprite(*e);
			}
		}
	}

	size_t skinClass::getImageCount()
	{
		return imagePath.size();
	}

	void skinClass::loadImages()
	{
		for (auto& path : imagePath)
			if (!path.empty())
			{
				if (path != "CONTINUE")
				{
					sf::Image img;
					if (img.loadFromFile(std::string(path.string())))
					{
						vecImage.push_back(std::move(img));
					}
					else
					{
						LOG(WARNING) << "[Skin] Image loading failed: " << path;
						vecImage.push_back(vecImage[ERROR_IMAGE]);
					}
				}
				else
				{
					// TODO CONTINUE
				}
			}
			else
			{
				vecImage.emplace_back();
			}
	}

	void skinClass::freeImages()
	{
		vecImage.clear();
	}

	void skinClass::convertImageToTexture()
	{
		for (auto& img : vecImage)
		{
			vecTexture.emplace_back();
			vecTexture.back().loadFromImage(img);
		}
	}

	void skinClass::createSprite(element& e)
	{
		if (e.empty()) return;

		if (e.getTextureIdx() > vecTexture.size())
		{
			switch (e.getTextureIdx())
			{
			case STAGEFILE:
				e.setTexture(texStageFile);
				break;
			case BACKBMP:
				e.setTexture(texBackBmp);
				break;
			case BANNER:
				e.setTexture(texBanner);
				break;
			case SKIN_THUMBNAIL:
				e.setTexture(texSkinThumbnail);
				break;
			case BLACK_DOT:
				e.setTexture(texBlackDot);
				break;
			case WHITE_DOT:
				e.setTexture(texWhiteDot);
				break;
			default:
				LOG(WARNING) << "[Skin] Image index undefined: " << e.getTextureIdx();
				e.setTexture(vecTexture[ERROR_TEXTURE]);
			}
		}
		else
		{
			e.setTexture(vecTexture[e.getTextureIdx()]);
		}

		e.createSprite();
	}

	void skinClass::updateSprite(element& e, long long time) const
	{
		e.update(e.getRTime(time));
	}

	void skinClass::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		long long rTime = data().getTimeFromStart();
		for (auto& e : elements)
		{
			if (e)
			{
				updateSprite(*e, rTime);
				e->draw(target, states);
			}
		}
	}

	bool skinClass::receiveInput()
	{
		return recvInput;
	}

	void skinClass::delayedStartReceiveInput()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(startInputTime));
		LOG(DEBUG) << "[Skin] Start receiving input";
		data().setTimer(timer::INPUT_START, data().getTimeFromStart());
		recvInput = true;
	}
}