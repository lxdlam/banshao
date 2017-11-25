#pragma once
#include <vector>
#include <list>
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <boost/tokenizer.hpp>
#include "element.h"
#include "../defs.h"
#include "../types.hpp"

namespace game
{
	using namespace defs::skin;
	using defs::general::eMode;
	using path = std::experimental::filesystem::path;

	class skinClass
	{
	public:
		skinClass();
		skinClass(path path, bool extended = false);
		~skinClass() = default;
		skinClass(skinClass&) = delete;
		skinClass& operator= (skinClass&) = delete;
		void init(path path, bool extended);

	protected:
		std::vector<path> imagePath;
		std::vector<path> lr2fontPath;

	protected:
		std::vector<sf::Image> vecImage;
		std::vector<sf::Texture> vecTexture;

		sf::Image imgStageFile;
		sf::Image imgBackBmp;
		sf::Image imgBanner;
		sf::Image imgSkinThumbnail;
		sf::Image imgBlackDot;
		sf::Image imgWhiteDot;
		sf::Texture texStageFile;
		sf::Texture texBackBmp;
		sf::Texture texBanner;
		sf::Texture texSkinThumbnail;
		sf::Texture texBlackDot;
		sf::Texture texWhiteDot;

	public:
		size_t getImageCount();
		void loadImages();
		void freeImages();
		void convertImageToTexture();

	private:
		size_t line = 0;
		typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
		std::vector<std::string> csvNextLineTokenize(std::istream&);

	#pragma region Element class definition
	public:


	protected:
		std::vector<std::shared_ptr<element>> elements;
    
    ///////////////////////////////////////////////////////////
	// public functions
	public:
		void createSprite(element& e);
		void updateSprite(element& e, long long time) const;
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    ///////////////////////////////////////////////////////////

	// LR2 skin
	private:
		int loadLR2image(const std::vector<std::string> &t);
		int loadLR2include(const std::vector<std::string> &t);
		int loadLR2font(const std::vector<std::string> &t);
		int loadLR2systemfont(const std::vector<std::string> &t);
		int loadLR2header(const std::vector<std::string> &t);
		int loadLR2timeoption(const std::vector<std::string> &t);
		int loadLR2others(const std::vector<std::string> &t);
		int loadLR2SkinLine(const std::vector<std::string> &t);

		int loadLR2src(const std::vector<std::string> &t);
		int loadLR2dst(const std::vector<std::string> &t);

		void loadLR2IF(const std::vector<std::string> &t, std::ifstream&);
		void loadLR2Skin(path path);

    ///////////////////////////////////////////////////////////


	protected:
		///////////////////////////////////////////////////////////
		// Info defined by header
		eMode mode;
		std::string title;
		std::string maker;

		// dst_op, title, option_labels, value
		std::vector<std::tuple<
			unsigned, 
			std::string, 
			std::vector<std::string>,
			unsigned>
		> customize;

		// title, file_path_list, default, value
		std::vector<std::tuple<
			std::string,
			std::vector<fs::path>, 
			size_t,
			size_t>
		> customFile;

		///////////////////////////////////////////////////////////

		bool recvInput = false;
		unsigned startInputTime = 0;
		unsigned resultStartInputTimeRank = 0;		// Result / Course Result Only
		unsigned resultStartInputTimeUpdate = 0;	// Result / Course Result Only
		unsigned startSkipTime = 0;					// Decide Only
		unsigned loadStartTime = 0;					// Play Skins Only
		unsigned loadTimeLengthThreshold = 0;		// Play Skins Only; from load start
		unsigned playStartAfterLoad = 0;			// Play Skins Only; from load end
		unsigned stageFailedTimeLength = 0;			// Play Skins Only
		unsigned fadeoutLength = 0;

		///////////////////////////////////////////////////////////
		
		bool reloadBanner = false;
		struct { unsigned r, g, b; } transColor = { 0, 255, 0 };
		bool flipSide = false;
		bool flipResult = false;					// Result / Course Result Only
		bool disableFlipResult = false;
		unsigned scratchSide1P = 0;					// Play Skins Only; 0: left, 1: right
		unsigned scratchSide2P = 0;					// Play Skins Only; 0: left, 1: right

		///////////////////////////////////////////////////////////
	public:
		bool receiveInput();
		void delayedStartReceiveInput();
	};
}