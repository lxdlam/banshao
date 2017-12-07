#pragma once
#include <list>
#include "data.h"
#include "../defs.h"
#include <SFML/Graphics.hpp>

namespace game
{
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

	using namespace defs::skin;

	class element: public sf::Sprite
	{
	private:
		void move(float offsetX, float offsetY);
		void move(const sf::Vector2f &offset);
		void rotate(float angle);
		void scale(float factorX, float factorY);
		void scale(const sf::Vector2f &factor);
	protected:
		void setColor(sf::Color &color);
		void setPosition(float x, float y);
		void setScale(float factorX, float factorY);
		void setOrigin(float x, float y);

	protected:
		elementType _type = elementType::GENERAL;
	public:
		elementType type() { return _type; }


	protected:
		size_t textureIdx = ERROR_TEXTURE;
		mutable bool bDraw = false;
		mutable sf::BlendMode blendMode;

		unsigned frames = 0;
		mutable size_t frame = 0;
		std::vector<sf::Sprite> sprites;

	protected:
		// Top-left point in the image
		unsigned x = 0, y = 0;

		// set -1 to use whole image
		// for videos, the value is forced -1
		int w = -1, h = -1;
		int subW, subH;

		// Source segmentation part count.
		// default is 1
		unsigned div_x = 1, div_y = 1;
		unsigned divCycleTime = 0;			// cycle
		unsigned divAnimStartTimer = 0;		// timer

	protected:
		std::vector<keyFrame> keyFrames;	// key frames lines

		int loopTo;						// loop
		timer timer;					// Start timer. See defs::skin::timer
		int dstOption[3];				// op1~3
		int scratchOp;					// op4

	public:
		size_t getTextureIdx() { return textureIdx; }
		void pushKeyFrame(int time, int x, int y, int w, int h, int acc, int r, int g, int b, int a, int blend, int filter, int angle, int center);
		void setDstParam(int loop, int timer, int op1, int op2, int op3, int op4);

	public:
		element(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer):
			 textureIdx(textureIdx), x(x), y(y), w(w), h(h), div_x(div_x), div_y(div_y), divCycleTime(cycle), divAnimStartTimer(timer) {}
		element() {}
		virtual ~element() {}

	public:
		virtual bool created() { return !sprites.empty(); }
		long long getRTime(long long time) { return data().getTimeFromTimer(timer, time); }
		bool empty() { return keyFrames.empty(); }
		void setBlendMode(int blend);
		virtual void createSprite();

		void update(long long time);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:
		virtual void checkDraw();
		virtual void updateSprites(float time, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle);
	};

	class elemNumber : public element
	{
	public:
		elemNumber(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer,
			unsigned num, unsigned align, unsigned digit):
			element(textureIdx, x, y, w, h, div_x, div_y, cycle, timer),
			num(num), alignType(align), digits(digit) { _type = elementType::NUMBER; }

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

	protected:
		bool negative;
		int displayNumBuffer = 0;
		std::list<size_t> displayNum;
		bool haveExtraZero = false;
		bool haveMinusNum = false;
		std::vector<std::vector<std::vector<sf::Sprite>>> numbers; // numbers[digit][num][frame]

	public:
		virtual bool created() { return !numbers.empty(); }
		virtual void createSprite();
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:
		virtual void updateSprites(float time, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle);
	};

	class elemSlider : public element
	{
	public:
		elemSlider(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer,
			unsigned muki, unsigned range, unsigned type, unsigned disable):
			element(textureIdx, x, y, w, h, div_x, div_y, cycle, timer),
			direction(muki), range(range), type(type), changeable(disable) { _type = elementType::SLIDER; }

	protected:
		// Slider direction (from lowest to highest)
		//  0: Up
		//  1: Right
		//  2: Down
		//  3: Left
		unsigned direction;

		// move range (by pixel, value range is defined internal)
		unsigned range;

		// Slider type representating, see defs::skin::slider
		unsigned type;

		unsigned changeable;

	protected:
		virtual void updateSprites(float time, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle);
	};

	class elemBargraph : public element
	{
	public:
		elemBargraph(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer,
			unsigned type, unsigned muki):
			element(textureIdx, x, y, w, h, div_x, div_y, cycle, timer),
			type(type), direction(muki) { _type = elementType::BARGRAPH; }

	protected:
		// Slider direction (from lowest to highest)
		//  0: Right
		//  1: Down
		unsigned direction;

		// Bargraph type representating, see defs::skin::bargraph
		unsigned type;

	public:
		virtual void updateSprites(float time, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle);
	};

	class elemButton : public element
	{
	public:
		elemButton(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer,
			unsigned type, unsigned click, int panel, int plusonly): 
			element(textureIdx, x, y, w, h, div_x, div_y, 0, timer),
			type(type), clickable(click), panel(panel), plusonly(plusonly) { _type = elementType::BUTTON; }

	protected:
		// Button type representating, see defs::skin::button
		unsigned type;

		bool clickable;
		int panel;
		int plusonly;

	// TODO click support

	protected:
		virtual void checkDraw();
		virtual void updateSprites(float time, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle);
	};

	class elemOnMouse : public element
	{
	public:
		elemOnMouse(size_t textureIdx, unsigned x, unsigned y, int w, int h, unsigned div_x, unsigned div_y, unsigned cycle, unsigned timer,
			int panel, int x2, int y2, int w2, int h2) :
			element(textureIdx, x, y, w, h, div_x, div_y, cycle, timer),
			panel(panel), x2(x2), y2(y2), w2(w2), h2(h2) { _type = elementType::ONMOUSE; }

	protected:
		int panel;
		int x2, y2, w2, h2;

	protected:
		virtual void checkDraw();
	};

	class elemText : public element
	{
	public:
		elemText() { _type = elementType::TEXT; }

	};

	#pragma endregion

}
