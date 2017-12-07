#include "data.h"
#include "../utils.h"
#include "../game/configManager.h"

namespace game
{
	dataClass::dataClass()
	{
		resetTimer();
		resetDstOption();
		resetNum();
		resetSlider();
		resetBargraph();
		resetButton();
	}

	dataClass& data()
	{
		static dataClass _inst;
		return _inst;
	}

	long long dataClass::getTimeFromStart()
	{
		return duration_cast<milliseconds>(system_clock::now() - skinStartTime).count() + 1;
	}

	long long dataClass::getTimeFromTimer(timer timer)
	{
		int rt = getTimeFromStart() - getTimer(timer);
		if (rt < 0)
			return -1;
		else
			return rt;
	}

	long long dataClass::getTimeFromTimer(timer timer, long long time)
	{
		int rt = time - getTimer(timer);
		if (rt < 0)
			return -1;
		else
			return rt;
	}

	void dataClass::setTimer(timer dst, long long src)
	{
		LOG(DEBUG) << "Timer " << static_cast<size_t>(dst) << " set to " << src;
		_timer[static_cast<size_t>(dst)] = src;
	}

	unsigned dataClass::getTimer(timer dst)
	{
		return _timer[static_cast<size_t>(dst)];
	}

	void dataClass::setDstOption(dst_option dst, bool src)
	{
		LOG(DEBUG) << "Opt " << static_cast<size_t>(dst) << " set to " << src;
		_dstOption[static_cast<size_t>(dst)] = src;
	}

	bool dataClass::getDstOption(dst_option dst)
	{
		return _dstOption[static_cast<size_t>(dst)];
	}

	bool dataClass::getDstOption(int dst)
	{
		if (abs(dst) > 999)
			return false;
		if (dst < 0)
			return !_dstOption[-dst];
		else
			return _dstOption[dst];
	}

	void dataClass::setNum(num dst, int src)
	{
		_num[static_cast<size_t>(dst)] = src;
	}

	int dataClass::getNum(num dst)
	{
		return _num[static_cast<size_t>(dst)];
	}

	void dataClass::resetTimer()
	{
		_timer.fill(0);
		skinStartTime = system_clock::now();
		setTimer(timer::SCENE_START, getTimeFromStart());
	}

	void dataClass::resetDstOption()
	{
		_dstOption.fill(false);
		setDstOption(dst_option::TRUE, true);
	}

	void dataClass::resetNum()
	{
		_num.fill(0);
	}

	void dataClass::setSlider(slider dst, unsigned src)
	{
		_slider[static_cast<size_t>(dst)] = src;
	}
	void dataClass::setSliderMax(slider dst, unsigned src)
	{
		_slider_max[static_cast<size_t>(dst)] = src;
	}
	unsigned dataClass::getSlider(slider dst)
	{
		return _slider[static_cast<size_t>(dst)];
	}
	unsigned dataClass::getSliderMax(slider dst)
	{
		return _slider_max[static_cast<size_t>(dst)];
	}

	void dataClass::resetSlider()
	{
		_slider.fill(0);
		_slider_max.fill(100);

		//setSlider(slider::VOLUME_MASTER, conf().audio.get("VolumeMaster"));
		setSlider(slider::VOLUME_MASTER, 100);
		setSlider(slider::VOLUME_KEY, 50);
		setSlider(slider::VOLUME_BGM, 75);

		setSliderMax(slider::SELECT_POSITION, 1);
		setSliderMax(slider::SONG_PROGRESS, 1000);
		setSliderMax(slider::SKINSELECT_CUSTOMIZE, 1);
		setSliderMax(slider::FX_PITCH, 12);
	}

	void dataClass::setBargraph(bargraph dst, float src)
	{
		_bargraph[static_cast<size_t>(dst)] = src;
	}

	float dataClass::getBargraph(bargraph dst)
	{
		return _bargraph[static_cast<size_t>(dst)];
	}

	void dataClass::resetBargraph()
	{
		_bargraph.fill(0);
	}

	void dataClass::setButton(button dst, unsigned src)
	{
		_button[static_cast<size_t>(dst)] = src;
	}

	unsigned dataClass::getButton(button dst)
	{
		return _button[static_cast<size_t>(dst)];
	}

	void dataClass::resetButton()
	{
		_button.fill(0);
	}
}