#include "element.h"

namespace game
{
	void element::pushKeyFrame(int time, int x, int y, int w, int h, int acc, int r, int g, int b, int a, int blend, int filter, int angle, int center)
	{
		keyFrames.push_back({
			static_cast<unsigned>(time),
			x, y, w, h,
			static_cast<unsigned>(acc),
			static_cast<unsigned>(r), static_cast<unsigned>(g), static_cast<unsigned>(b), static_cast<unsigned>(a),
			static_cast<unsigned>(blend), static_cast<unsigned>(filter),
			angle, static_cast<unsigned>(center)});
	}

	void element::setDstParam(int loop, int timer, unsigned op1, unsigned op2, unsigned op3, unsigned op4)
	{
		loopTo = loop;
		this->timer = static_cast<defs::skin::timer>(timer);
		dstOption[0] = op1;
		dstOption[1] = op2;
		dstOption[2] = op3;
		scratchOp = op4;
	}

	void element::setBlendMode(int b)
	{
		// blend mode
		using Eq = sf::BlendMode::Equation;
		using Factor = sf::BlendMode::Factor;
		blend pb = static_cast<blend>(b);
		switch (pb)
		{
		case blend::TRANSCOLOR:
			blendMode = sf::BlendNone;
			break;

		case blend::ALPHA:
			blendMode = sf::BlendAlpha;
			break;

		case blend::ADD:
			blendMode = sf::BlendAdd;
			break;

		case blend::MULTIPLY:
			blendMode = sf::BlendMultiply;
			break;

		case blend::SUBTRACT:
			blendMode = sf::BlendMode(
				Factor::SrcAlpha, Factor::One, Eq::ReverseSubtract,
				Factor::One, Factor::One, Eq::Add);
			break;

		case blend::ANTI_COLOR:
			blendMode = sf::BlendMode(
				Factor::Zero, Factor::OneMinusSrcColor);
			break;

		case blend::MULTIPLY_ANTI_BACKGROUND:
			blendMode = sf::BlendMode(
				Factor::OneMinusDstColor, Factor::Zero);
			break;

		case blend::MULTIPLY_WITH_ALPHA:
			blendMode = sf::BlendMode(
				Factor::DstColor, Factor::Zero, Eq::Add,
				Factor::DstAlpha, Factor::Zero, Eq::Add);
			break;

		case blend::XOR:
			blendMode = sf::BlendMode(
				Factor::OneMinusDstColor, Factor::OneMinusSrcColor, Eq::Add,
				Factor::One, Factor::One, Eq::Add
			);
			break;
		}
	}

	void element::update(long long rTime)
	{
		if (!created() || rTime == -1)
			return;

		bDraw = data().getDstOption(static_cast<dst_option>(dstOption[0])) &&
			data().getDstOption(static_cast<dst_option>(dstOption[1])) &&
			data().getDstOption(static_cast<dst_option>(dstOption[2]));
		if (!bDraw)
			return;

		unsigned lastKFTime = keyFrames[keyFrames.size() - 1].time;
		if (data().getTimer(timer) == 0 || 
			rTime > lastKFTime && (loopTo == -1 || loopTo > lastKFTime) ||
			w == 0 || h == 0)
		{
			bDraw = false;
			return;
		}

		if (rTime > lastKFTime)
		{
			if (lastKFTime != loopTo)
				rTime = (rTime - loopTo) % (lastKFTime - loopTo) + loopTo;
			else
				rTime = loopTo;
		}

		// get keyFrame section
		unsigned keyFrameIdx = -1, keyFrameNext = -1;
		for (size_t f = 0; f < keyFrames.size(); ++f)
			if (keyFrames[f].time <= rTime)
				keyFrameIdx = f;

		// normalize time
		if (keyFrameIdx == -1)
		{
			bDraw = false;
			return;
		}
		else if (keyFrameIdx == keyFrames.size() - 1)
		{
			keyFrameNext = keyFrameIdx;
		}
		else
		{
			keyFrameNext = keyFrameIdx + 1;
		}

		auto& curr = keyFrames[keyFrameIdx];
		auto& next = keyFrames[keyFrameNext];
		float timeFactor = 1.0f;
		unsigned keyFrameLength = next.time - curr.time;
		if (keyFrameLength > 0)
			timeFactor = 1.0f * rTime / keyFrameLength;

		// calculate parameters
		float x = curr.x + (next.x - curr.x) * timeFactor;
		float y = curr.y + (next.y - curr.y) * timeFactor;
		float w = (curr.w + (next.w - curr.w) * timeFactor) / subW;		// -1 / -1 = 1
		float h = (curr.h + (next.h - curr.h) * timeFactor) / subH;
		unsigned r = static_cast<unsigned>(curr.r + (next.r - curr.r) * timeFactor);
		unsigned g = static_cast<unsigned>(curr.g + (next.g - curr.g) * timeFactor);
		unsigned b = static_cast<unsigned>(curr.b + (next.b - curr.b) * timeFactor);
		unsigned a = static_cast<unsigned>(curr.a + (next.a - curr.a) * timeFactor);
		float angle = curr.rotateAngle + (next.rotateAngle - curr.rotateAngle) * timeFactor;
		frame = (divCycleTime == 0) ? 0 : rTime / divCycleTime % frames;

		updateSprites(timeFactor, x, y, w, h, r, g, b, a, angle);
		setBlendMode(curr.blend);
	}


	void element::createSprite()
	{
		auto& texture = *getTexture();
		auto texSize = texture.getSize();
		//if (x + w > texSize.x) w = texSize.x - x;
		//if (y + h > texSize.y) h = texSize.y - y;
		subW = w / div_x;
		subH = h / div_y;
		frames = div_x * div_y;

		// divx divy
		if (div_x > 1 || div_y > 1)
		{
			sprites.resize(frames);
			for (int row = 0; row < div_y; ++row)
				for (int col = 0; col < div_x; ++col)
				{
					auto& spr = sprites[row * div_x + col];
					spr = sf::Sprite(texture, sf::IntRect(
						x + col * subW,
						y + row * subH,
						subW, subH
					));
				}
		}
		else
		{
			sprites.clear();
			sprites.emplace_back(texture, sf::IntRect(x, y, subW, subH));
		}

		// TODO timers?

	}

	void element::updateSprites(float timeFactor, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle)
	{
		sprites[frame].setPosition(x, y);
		sprites[frame].setScale(w, h);
		sprites[frame].setColor(sf::Color(r, g, b, a));
		sprites[frame].setRotation(angle);
	}

	void element::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!bDraw) return;
		states.blendMode = blendMode;
		target.draw(sprites[frame], states);
	}


	void elemNumber::createSprite()
	{
		unsigned numType = 10;
		int div = div_x * div_y;
		if (div % 24 == 0)
		{
			haveMinusNum = true;
			haveExtraZero = true;
			numType = 24;
		}
		else if (div % 11 == 0)
		{
			haveExtraZero = true;
			numType = 11;
		}
		else if (div % 10 != 0)
		{
			return;
		}

		auto& texture = *getTexture();
		auto texSize = texture.getSize();
		if (x + w > texSize.x) w = texSize.x - x;
		if (y + h > texSize.y) h = texSize.y - y;

		frames = div / numType;

		numbers.resize(digits);
		for (auto& n : numbers)
		{
			n.resize(numType);
			for (auto& f : n)
			{
				f.resize(frames);
			}
		}

		unsigned nums = numType;
		if (haveMinusNum) nums = 24;

		// divx divy
		subW = w / div_x;
		subH = h / div_y;
		int row = 0;
		int col = 0;
		for (unsigned d = 0; d < digits; ++d)
			for (unsigned num = 0; num < nums; ++num)
				for (unsigned frame = 0; frame < frames; ++frame)
				{
					if (num != 11 && num != 23) 	// skip +-
						numbers[d][num][frame] = sf::Sprite(
							texture, sf::IntRect(
								x + col * subW,
								y + row * subH,
								subW, subH));
					if (++row == div_y) row = 0;
					if (++col == div_x) col = 0;
				}

		// +-
		if (haveMinusNum)
		{
			numbers.emplace_back();
			auto& symbol = numbers[digits];
			symbol.resize(2);
			symbol[0].resize(frames);
			symbol[1].resize(frames);
			for (unsigned frame = 0; frame < frames; ++frame)
			{
				int tex = (frame * 2 + 1) * 12 - 1;
				symbol[0][frame] = sf::Sprite(
					texture, sf::IntRect(
						x + tex % div_x * subW,
						y + tex / div_x * subH,
						subW, subH));

				tex += 12;
				symbol[1][frame] = sf::Sprite(
					texture, sf::IntRect(
						x + tex % div_x * subW,
						y + tex / div_x * subH,
						subW, subH));
			}
		}

		// TODO timers?

	}

	void elemNumber::updateSprites(float timeFactor, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle)
	{
		// FIXME replace displayNum with a static array
		// get number
		int number = data().getNum(static_cast<defs::skin::num>(num));
		if (number != displayNumBuffer || displayNum.empty())
		{
			displayNum.clear();
			negative = number < 0;
			do
			{
				displayNum.push_back(number % 10);
				number /= 10;
			} while (number != 0 && displayNum.size() < digits);

			// zeros
			size_t size = displayNum.size();
			int blanks = digits - size;

			if (alignType == 0)
				for (unsigned d = 0; d < blanks; ++d)
					displayNum.push_back(haveMinusNum && negative ? 22 : 10);

			for (unsigned d = 0; d < size; ++d)
			{
				size_t n = displayNum.front();
				displayNum.pop_front();
				displayNum.push_back(haveMinusNum && negative ? n + 12 : n);
			}

			if (alignType != 0)
				for (unsigned d = 0; d < blanks; ++d)
					displayNum.push_back(haveMinusNum && negative ? 22 : 10);
		}

		displayNumBuffer = number;
		if (haveMinusNum) x += w * subW;
		if (alignType == 2) x += w * subW * (digits - displayNum.size()) / 2;

		// symbol
		if (haveMinusNum)
		{
			unsigned n = negative ? 1 : 0;
			auto& spr = numbers[digits][n][frame];
			spr.setPosition(x - w * subW, y);
			spr.setScale(w, h);
			spr.setColor(sf::Color(r, g, b, a));
			spr.setRotation(angle);
		}

		// numbers
		auto it = displayNum.begin();
		for (size_t d = 0; d < digits; ++d, ++it)
		{
			unsigned n = haveMinusNum && negative ? *it + 12 : *it;
			if (n == 10 && !haveExtraZero)
				continue;
			auto& spr = numbers[d][n][frame];
			spr.setPosition(x + d * w * subW, y);
			spr.setScale(w, h);
			spr.setColor(sf::Color(r, g, b, a));
			spr.setRotation(angle);
		}

	}

	void elemNumber::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!bDraw) return;
		states.blendMode = blendMode;

		// symbol
		if (haveMinusNum)
			target.draw(numbers[digits][negative ? 1 : 0][frame], states);

		// numbers
		auto it = displayNum.begin();
		for (size_t d = 0; d < digits; ++d, ++it)
			if (*it != 10 || *it == 10 && haveExtraZero)
				target.draw(numbers[d][*it][frame], states);
	}


	void elemSlider::updateSprites(float timeFactor, float x, float y, float w, float h, unsigned r, unsigned g, unsigned b, unsigned a, float angle)
	{
		slider ty = static_cast<slider>(type);
		float var = std::floorf(1.0f * data().getSlider(ty) / data().getSliderMax(ty) * range);
		switch (direction)
		{
		case 0:			// up
			y -= var;
			break;
		case 1:			// right
			x += var;
			break;
		case 2:			// down
			y += var;
			break;
		case 3:			// left
			x -= var;
			break;
		}

		element::updateSprites(timeFactor, x, y, w, h, r, g, b, a, angle);
	}

}