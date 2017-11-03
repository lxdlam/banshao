#pragma once
#include <vector>
#include <list>
#include <string>
#include <filesystem>
#include <SFML/Graphics.hpp>
#include <boost/tokenizer.hpp>
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
		void convertImageToTexture();

	private:
		size_t line = 0;
		typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
		std::vector<std::string> csvNextLineTokenize(std::istream&);

	#pragma region Element class definition
	public:
		class element
		{
		private:
			elementType type = elementType::GENERAL;

		public:
			virtual ~element() {}
			friend class skinClass;
			struct keyFrame					// mostly come from #DST_IMAGE
			{
				unsigned time;				// keyframe time mark
				int x, y, w, h;
				unsigned accelerateType;	// acc, 0~3: constant / accel / decel / discontinuous
				unsigned r, g, b, a;
				unsigned blend;				// blend, see defs::skin::blend
				unsigned zoomFilter;		// filter
				int rotateAngle;			// angle, 360 for one lap
				unsigned alignCenter;		// center, values are keypad corner
				
				//bool move = false;
				//bool color = false;
				//bool rotate = false;
			};

		protected:
			size_t textureIdx;
			mutable bool draw = false;
			mutable sf::Sprite sprite;
			mutable sf::BlendMode blendMode;

		#pragma region SRC_IMAGE definitions
		protected:
			size_t imageIdx;

			// Top-left point in the image
			unsigned x, y;

			// set -1 to use whole image
			// for videos, the value is forced -1
			int w, h;

			// Source segmentation part count.
			// default is 1
			unsigned div_x, div_y;
			unsigned divCycleTime;			// cycle
			unsigned divAnimStartTimer;		// timer

		#pragma endregion

		#pragma region DST_IMAGE definitions
		protected:
			// Key frame lines
			std::vector<keyFrame> keyFrames;

			int loopTo;						// loop

			timer timer;					// Start timer. See defs::skin::timer

			unsigned dstOption[3];			// op1~3
			unsigned scratchOp;				// op4

		#pragma endregion

		};

		class elemNumber : public element
		{
		public:
			friend class skinClass;
			elemNumber() { type = elementType::NUMBER; }

		// SRC NUMBER Definitions
		protected:

			// Data representating, see defs::skin::num
			unsigned num;

			// number align type | align
			//	0: Right
			//	1: Left
			//	2: Center
			unsigned alignType;

			// Max number of digits | keta
			unsigned digits;
		};

		class elemSlider : public element
		{
		public:
			elemSlider() { type = elementType::SLIDER; }
		};

		class elemBargraph : public element
		{
		public:
			elemBargraph() { type = elementType::BARGRAPH; }
		};

		class elemButton : public element
		{
		public:
			elemButton() { type = elementType::BUTTON; }
		};

		class elemOnMouse : public element
		{
		public:
			elemOnMouse() { type = elementType::ONMOUSE; }

		};

		class elemText : public element
		{
		public:
			elemText() { type = elementType::TEXT; }

		};

	#pragma endregion

	protected:
		std::vector<std::shared_ptr<element>> elements;
    
    ///////////////////////////////////////////////////////////
	// public functions
	public:
		void createSprite(element& e);
		void updateSprite(element& e) const;
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
	};
}