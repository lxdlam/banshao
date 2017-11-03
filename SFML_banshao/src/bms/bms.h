#pragma once
#include <array>
#include <string>
#include <list>
#include <map>

#include "../defs.h"
#include "../utils.h"
using utils::base16;
using utils::base36;

namespace game
{
	using namespace game::defs::bms;

	class bms
	{
	public:
		// TODO replace string_t with pathType
		typedef std::u32string string_t;
		bms() = default;
		~bms() = default;
		int initWithFile(std::string file);
		std::string getError();

	protected:
		bool initialized = false;
		bmsErrorCode errorCode = bmsErrorCode::OK;
		int errorLine;

	public:
		struct channel {
			// {segment, sampleIdx/value}
			std::list<std::pair<unsigned, unsigned>> notes{};
			int segments = 1;
		};

	protected:
		// BMS File Path.
		std::string path;
		std::string dirpath;

		// File properties.
		// TODO MD5_t MD5;

		// Header.
		int player = 1;				// 1: single, 2: couple, 3: double, 4: battle
		int rank = 2;				// judge, VHARD/HARD/NORMAL/EASY
		float total;
		int playLevel = 0;
		int difficulty = 3;			// N/H/A
		double bpm = 130.0;
		string_t title;
		string_t subTitle;
		string_t artist;
		string_t subArtist;
		string_t genre;
		std::map<std::string, string_t> extraCommands;

		// File assigned by the bms file.
		std::string stagefile;			// BG, 640x480
		std::string banner;			// banner, 300x80
		std::array<std::string, MAXSAMPLEIDX + 1> wavFile{};
		std::array<std::string, MAXSAMPLEIDX + 1> bmpFile{};

		// Measures related.
		std::array<double, MAXSAMPLEIDX + 1> exBPM{};
		std::array<double, MAXSAMPLEIDX + 1> stop{};
		std::array<double, MAXMEASUREIDX + 1> barLength{};
		
		// Channels.
		int strToChannel36(channel&, const std::string& str);
		int strToChannel16(channel&, const std::string& str);
		std::array<unsigned, MAXMEASUREIDX+ 1> bgmLayersCount{};
		std::array<std::array<channel, MAXMEASUREIDX + 1>, BGMCHANNELS> chBGM{};
		std::array<channel, MAXMEASUREIDX + 1> chStop{};
		std::array<channel, MAXMEASUREIDX + 1> chBPMChange{};
		std::array<channel, MAXMEASUREIDX + 1> chExBPMChange{};
		std::array<channel, MAXMEASUREIDX + 1> chBGABase{};
		std::array<channel, MAXMEASUREIDX + 1> chBGALayer{};
		std::array<channel, MAXMEASUREIDX + 1> chBGAPoor{};
		std::array<std::array<channel, MAXMEASUREIDX + 1>, 20> chNotesVisible{};
		std::array<std::array<channel, MAXMEASUREIDX + 1>, 20> chNotesInvisible{};
		std::array<std::array<channel, MAXMEASUREIDX + 1>, 20> chNotesLN{};
		std::array<std::array<channel, MAXMEASUREIDX + 1>, 20> chMines{};
		int strToNoteChannelDispatcher(decltype(chNotesVisible)&, int measure, int layer, int ch, const std::string& str);

		// Properties detected when parsing.
		bool haveNote = false;
		bool have67 = false;
		bool have89 = false;
		bool haveLN = false;
		bool haveMine = false;
		bool haveInvisible = false;
		bool haveBarChange = false;
		bool haveStop = false;
		bool haveBPMChange = false;
		bool haveBGA = false;
		bool haveRandom = false;
		unsigned bgmLayers = 0;
		unsigned long notes = 0;
		unsigned maxMeasure = 0;
		double minBPM;
		double maxBPM;

	public:
		int getMode() const;
		unsigned getNoteCount() const;
		unsigned getMaxMeasure() const;
		bool hasMine() const;
		bool hasLN() const;
		bool hasInvisible() const;
		bool hasBPMchange() const;
		bool hasStop() const;
		bool hasBGA() const;
		bool hasRandom() const;
		double getMinBpm() const;
		double getMaxBpm() const;

		string_t getTitle() const;
		string_t getSubTitle() const;
		string_t getArtist() const;
		string_t getSubArtist() const;
		string_t getGenre() const;

		double getBPM() const;
		int getJudgeRank() const;
		int getPlayLevel() const;
		int getDifficulty() const;

		auto getMeasureLength(unsigned idx) const -> decltype(barLength[0]) const;
		unsigned getBGMChannelCount(unsigned measure) const;
		auto getChannel(bmsGetChannelCode, unsigned chIdx, unsigned measureIdx) const -> const decltype(chBGM[0][0])&;

		auto getExBPM(size_t idx) const -> decltype(exBPM[0]);
		auto getStop(size_t idx) const -> decltype(stop[0]);

		auto getWavPath(size_t idx) const -> decltype(wavFile[0]);
		auto getBmpPath(size_t idx) const -> decltype(bmpFile[0]);

		std::string getDirectory() const;
	};
}