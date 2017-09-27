#include "bms.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <utility>
#include <exception>
#include <filesystem>

using utils::base16;
using utils::base36;

namespace game
{
	class noteLineException : public std::exception {};

	int bms::initWithFile(std::string file)
	{
		using err = defs::bmsErrorCode;
		if (initialized)
		{
			//errorCode = err::ALREADY_INITIALIZED;
			//errorLine = 0;
			return 1;
		}

		auto p = std::experimental::filesystem::absolute(file);
		path = p.string();
		dirpath = p.parent_path().string();
		std::ifstream fs(path);
		if (fs.fail())
		{
			errorCode = err::FILE_ERROR;
			errorLine = 0;
			return 1;
		}


		// TODO 天国的PARSER！！！
		// SUPER LAZY PARSER FOR TESTING
		std::vector<std::regex> skipRegex;
		skipRegex.emplace_back(R"(\*-.*)");
		skipRegex.emplace_back(R"(\/\/.*)");
		skipRegex.emplace_back(R"(;.*)");
		skipRegex.emplace_back(R"(#BPM00 .*)");
		skipRegex.emplace_back(R"(#STOP00 .*)");
		skipRegex.emplace_back(R"(#BMP00 .*)");

		std::string buf;
		unsigned line = 0;
		while (!fs.eof())
		{
			std::getline(fs, buf, '\n');
			line++;
			if (buf.length() <= 1) continue;

			buf = buf.substr(buf.find_first_not_of(' '),  buf.find_first_of('\r') - buf.find_first_not_of(' '));

			bool skip = false;
			for (auto& reg: skipRegex)
				if (std::regex_match(buf, reg))
				{
					skip = true;
					break;
				}
			if (skip) continue;

			if (buf[0] == '#')
			{
				auto space_idx = buf.find_first_of(' ');
				auto colon_idx = buf.find_first_of(':');
				if (space_idx == 1 || colon_idx == 1) continue;
				if (space_idx == buf.npos && colon_idx == buf.npos) continue;

				try {

					if (space_idx != buf.npos)
					{
						std::string key = buf.substr(1, space_idx - 1);
						std::string value = buf.substr(space_idx + 1);
						if (value.empty()) continue;

						// digits
						if (key == "PLAYER")
							player = std::stoi(value);
						else if (key == "RANK")
							rank = std::stoi(value);
						else if (key == "TOTAL")
							total = std::stof(value);
						else if (key == "PLAYLEVEL")
							playLevel = std::stoi(value);
						else if (key == "DIFFICULTY")
							difficulty = std::stoi(value);
						else if (key == "BPM")
							bpm = std::stod(value);

						// strings
						else if (key == "TITLE")
							title.assign(value.begin(), value.end());
						else if (key == "SUBTITLE")
							subTitle.assign(value.begin(), value.end());
						else if (key == "ARTIST")
							artist.assign(value.begin(), value.end());
						else if (key == "SUBARTIST")
							subArtist.assign(value.begin(), value.end());
						else if (key == "GENRE")
							genre.assign(value.begin(), value.end());
						else if (key == "STAGEFILE")
							stagefile.assign(value.begin(), value.end());
						else if (key == "BANNER")
							banner.assign(value.begin(), value.end());
						else if (key == "RANDOM" || key == "RONDAM")
							; // TODO #RANDOM

						// #xxx00
						else if (std::regex_match(key, std::regex(R"(WAV[0-9A-Za-z]{1,2})")))
						{
							int idx = base36(key[3], key[4]);
							wavFile[idx].assign(value.begin(), value.end());
						}
						else if (std::regex_match(key, std::regex(R"(BMP[0-9A-Za-z]{1,2})")))
						{
							int idx = base36(key[3], key[4]);
							bmpFile[idx].assign(value.begin(), value.end());
						}
						else if (std::regex_match(key, std::regex(R"(BPM[0-9A-Za-z]{1,2})")))
						{
							int idx = base36(key[3], key[4]);
							exBPM[idx] = std::stod(value);
						}
						else if (std::regex_match(key, std::regex(R"(STOP[0-9A-Za-z]{1,2})")))
						{
							int idx = base36(key[4], key[5]);
							stop[idx] = std::stod(value);
						}

						// unknown
						else
							extraCommands[key].assign(value.begin(), value.end());
					}

					else if (colon_idx != buf.npos)
					{
						std::string key = buf.substr(1, colon_idx - 1);
						std::string value = buf.substr(colon_idx + 1);
						if (value.empty())
						{
							LOG(WARNING) << "[BMS] Empty element line detected: line " << line;
							errorLine = line;
							errorCode = defs::bmsErrorCode::NOTE_LINE_ERROR;
							return 1;
						}

						// maxMeasure & channels
						if (std::regex_match(key, std::regex(R"(\d{3}[0-9A-Za-z]{2})")))
						{
							unsigned measure = std::stoi(key.substr(0, 3));
							if (maxMeasure < measure)
								maxMeasure = measure;
							std::pair<int, int> channel = { base36(key[3]), base36(key[4]) };
							switch (channel.first)
							{
							case 0:			// 0x: basic info
								switch (channel.second)
								{
								case 1:			// 01: BGM
									strToChannel36(chBGM[bgmLayersCount[measure]++][measure], value);
									if (bgmLayersCount[measure] > bgmLayers)
										bgmLayers = bgmLayersCount[measure];
									break;

								case 2:			// 02: Measure Length
									barLength[measure] = std::stod(value);
									haveBarChange = true;
									break;

								case 3:			// 03: BPM change
									strToChannel16(chBPMChange[measure], value);
									haveBPMChange = true;
									break;

								case 4:			// 04: BGA Base
									strToChannel36(chBGABase[measure], value);
									haveBGA = true;
									break;

								case 6:			// 06: BGA Poor
									strToChannel36(chBGAPoor[measure], value);
									haveBGA = true;
									break;

								case 7:			// 07: BGA Layer
									strToChannel36(chBGALayer[measure], value);
									haveBGA = true;
									break;

								case 8:			// 08: ExBPM
									strToChannel36(chExBPMChange[measure], value);
									haveStop = true;
									break;

								case 9:			// 09: Stop
									strToChannel36(chStop[measure], value);
									haveStop = true;
									break;
								}
								break;

							case 1:			// 1x: 1P visible
							case 2:			// 2x: 2P visible
								strToNoteChannelDispatcher(chNotesVisible, measure, channel.first, channel.second, value);
								haveNote = true;
								break;
							case 3:			// 3x: 1P invisible
							case 4:			// 4x: 2P invisible
								strToNoteChannelDispatcher(chNotesInvisible, measure, channel.first, channel.second, value);
								haveInvisible = true;
								break;
							case 5:			// 5x: 1P LN
							case 6:			// 6x: 2P LN
								strToNoteChannelDispatcher(chNotesLN, measure, channel.first, channel.second, value);
								haveLN = true;
								break;
							case 0xD:		// Dx: 1P mine
							case 0xE:		// Ex: 2P mine
								strToNoteChannelDispatcher(chMines, measure, channel.first, channel.second, value);
								haveMine = true;
								break;
							}
						}
					}
				}
				catch (noteLineException)
				{
					errorCode = err::NOTE_LINE_ERROR;
					throw;
				}
				catch (std::invalid_argument)
				{
					errorCode = err::TYPE_MISMATCH;
					throw;
				}
				catch (std::out_of_range)
				{
					errorCode = err::VALUE_ERROR;
					throw;
				}
				catch (std::exception)
				{
					fs.close();
					errorLine = line;
					return 1;
				}
			}
		}

		fs.close();

		// Get statistics
		for (size_t i = 0; i <= maxMeasure; i++)
			if (barLength[i] == 0.0)
				barLength[i] = 1.0;

		if (haveNote)
		for (const auto& chs : chNotesVisible)
			for (unsigned m = 0; m <= maxMeasure; m++)
				for (const auto& ns : chs[m].notes)
					notes++;

		if (haveLN)
		{
			int ln = 0;
			for (const auto& chs : chNotesLN)
				for (unsigned m = 0; m <= maxMeasure; m++)
					for (const auto& ns : chs[m].notes)
						ln++;
			notes += ln / 2;
		}

		minBPM = bpm;
		maxBPM = bpm;
		if (haveBPMChange)
		{
			for (unsigned m = 0; m <= maxMeasure; m++)
			{
				for (const auto& ns : chBPMChange[m].notes)
				{
					if (ns.second > maxBPM) maxBPM = ns.second;
					if (ns.second < minBPM) minBPM = ns.second;
				}
				for (const auto& ns : chExBPMChange[m].notes)
				{
					if (exBPM[ns.second] > maxBPM) maxBPM = exBPM[ns.second];
					if (exBPM[ns.second] < minBPM) minBPM = exBPM[ns.second];
				}
			}
		}

		initialized = true;

		return 0;
	}
	
	int bms::strToChannel36(channel& ch, const std::string& str)
	{
		if (!std::regex_match(str, std::regex(R"(([0-9A-Za-z][0-9A-Za-z]){1,})")))
			throw new noteLineException;

		ch.segments = str.length() / 2;
		for (int i = 0; i < ch.segments; i++)
			if (unsigned sample = base36(str[i * 2], str[i * 2 + 1]))
				ch.notes.push_back({ i, sample });
		return 0;
	}

	int bms::strToChannel16(channel& ch, const std::string& str)
	{
		if (!std::regex_match(str, std::regex(R"(([0-9A-Fa-f][0-9A-Fa-f]){1,})")))
			throw new noteLineException;

		ch.segments = str.length() / 2;
		for (int i = 0; i < ch.segments; i++)
			if (unsigned sample = base16(str[i * 2], str[i * 2 + 1]))
				ch.notes.push_back({ i, sample });
		return 0;
	}
	
	int bms::strToNoteChannelDispatcher(decltype(chNotesVisible)& arrCh, int measure, int layer, int ch, const std::string& str)
	{
		int idx = 0;
		switch (layer)
		{
			case 1:			// 1x: 1P visible
			case 3:			// 3x: 1P invisible
			case 5:			// 5x: 1P LN
			case 0xD:		// Dx: 1P mine
				idx = 0;
				break;
			case 2:			// 2x: 2P visible
			case 4:			// 4x: 2P invisible
			case 6:			// 6x: 2P LN
			case 0xE:		// Ex: 2P mine
				idx = 10;
				break;
		}
		switch (ch)
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			idx += ch;
			break;
		case 6:		//SCR
			break;
		case 8:		//6
		case 9:		//7
			have67 = true;
			idx += 6 + ch - 8;
			break;
		case 7:		//9 of PMS BME
			have89 = true;
			idx = 9;
			break;
		}
		return strToChannel36(arrCh[idx][measure], str);
	}

	std::string bms::getError()
	{
		using err = defs::bmsErrorCode;
		switch (errorCode)
		{
		case err::OK:	return "No errors.";
			//TODO return translated strings 
		}

		return "?";
	}
	
	int bms::getMode() const
	{
		if (player == 3)
			return (have67) ? defs::MODE_14KEYS : defs::MODE_10KEYS;
		else
			return (have89) ? defs::MODE_9KEYS : (have67) ? defs::MODE_7KEYS : defs::MODE_5KEYS;
	}

	unsigned bms::getNoteCount() const
	{
		return notes;
	}

	unsigned bms::getMaxMeasure() const
	{
		return maxMeasure;
	}

	bool bms::hasMine() const
	{
		return haveMine;
	}

	bool bms::hasLN() const
	{
		return haveLN;
	}

	bool bms::hasInvisible() const
	{
		return haveInvisible;
	}

	bool bms::hasBPMchange() const
	{
		return haveBPMChange;
	}

	bool bms::hasStop() const
	{
		return haveStop;
	}

	bool bms::hasBGA() const
	{
		return haveBGA;
	}

	bool bms::hasRandom() const
	{
		return haveRandom;
	}

	double bms::getMinBpm() const
	{
		return minBPM;
	}

	double bms::getMaxBpm() const
	{
		return maxBPM;
	}


	auto bms::getTitle() const -> string_t
	{
		return title;
	}

	auto bms::getSubTitle() const -> string_t
	{
		return subTitle;
	}

	auto bms::getArtist() const -> string_t
	{
		return artist;
	}

	auto bms::getSubArtist() const -> string_t
	{
		return subArtist;
	}

	auto bms::getGenre() const -> string_t
	{
		return genre;
	}


	double bms::getBPM() const
	{
		return bpm;
	}

	int bms::getJudgeRank() const
	{
		return rank;
	}

	int bms::getPlayLevel() const
	{
		return playLevel;
	}

	int bms::getDifficulty() const
	{
		return difficulty;
	}

	auto bms::getMeasureLength(unsigned idx) const -> decltype(barLength[0]) const
	{
		return barLength[idx];
	}

	unsigned bms::getBGMChannelCount(unsigned measure) const
	{
		return bgmLayersCount[measure];
	}
	
	auto bms::getChannel(defs::bmsGetChannelCode code, unsigned chIdx, unsigned measureIdx) const -> const decltype(chBGM[0][0])&
	{
		using eC = defs::bmsGetChannelCode;
		switch (code)
		{
		case eC::BGM:		return chBGM[chIdx][measureIdx]; break;
		case eC::BPM:		return chBPMChange[measureIdx]; break;
		case eC::EXBPM:		return chExBPMChange[measureIdx]; break;
		case eC::STOP:		return chStop[measureIdx]; break;
		case eC::BGABASE:	return chBGABase[measureIdx]; break;
		case eC::BGALAYER:	return chBGALayer[measureIdx]; break;
		case eC::BGAPOOR:	return chBGAPoor[measureIdx]; break;;
		case eC::NOTE1:		return chNotesVisible[chIdx][measureIdx]; break;
		case eC::NOTE2:		return chNotesVisible[10 + chIdx][measureIdx]; break;
		case eC::NOTEINV1:	return chNotesInvisible[chIdx][measureIdx]; break;
		case eC::NOTEINV2:	return chNotesInvisible[10 + chIdx][measureIdx]; break;
		case eC::NOTELN1:	return chNotesLN[chIdx][measureIdx]; break;
		case eC::NOTELN2:	return chNotesLN[10 + chIdx][measureIdx]; break;
		case eC::NOTEMINE1:	return chMines[chIdx][measureIdx]; break;
		case eC::NOTEMINE2:	return chMines[10 + chIdx][measureIdx]; break;
		}
		// FIXME warning C4715 : “game::bms::getChannel” : 不是所有的控件路径都返回值
	}

	auto bms::getExBPM(size_t idx) const -> decltype(exBPM[0])
	{
		return exBPM[idx];
	}
	auto bms::getStop(size_t idx) const -> decltype(stop[0])
	{
		return stop[idx];
	}

	auto bms::getWavPath(size_t idx) const -> decltype(wavFile[0])
	{
		return wavFile[idx];
	}
	auto bms::getBmpPath(size_t idx) const -> decltype(bmpFile[0])
	{
		return bmpFile[idx];
	}

	std::string bms::getDirectory() const
	{
		return dirpath;
	}
}