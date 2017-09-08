#include "sound.h"
#include "../Config/audio.h"
#include "../utils.h"
#include <fmod_errors.h>

using utils::log;

namespace game
{
	Sound::Sound()
	{
		FMOD_RESULT r;

		r = FMOD::System_Create(&fmodSystem);      // Create the main system object.
		if (r != FMOD_OK)
		{
			log("[FMOD] Error: Create FMOD System Failed: " + std::string(FMOD_ErrorString(r)));
			return;
		}

		fmodSystem->setDSPBufferSize(
			Config::audio::getInstance().getBufferLength(),
			Config::audio::getInstance().getBufferCount()
		);

		r = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
		if (r != FMOD_OK)
		{
			log("[FMOD] Error: FMOD System Initialize Failed: " + std::string(FMOD_ErrorString(r)));
			return;
		}

		initResult = { r, FMOD_ErrorString(r) };

		if (r == FMOD_OK)
		{
			FMOD_OUTPUTTYPE outputtype;
			fmodSystem->getOutput(&outputtype);
			switch (outputtype)
			{
			case FMOD_OUTPUTTYPE_AUTODETECT: log("[FMOD] Output Type: AUTODETECT"); break;
			case FMOD_OUTPUTTYPE_UNKNOWN: log("[FMOD] Output Type: UNKNOWN"); break;
			case FMOD_OUTPUTTYPE_NOSOUND: log("[FMOD] Output Type: NOSOUND"); break;
			case FMOD_OUTPUTTYPE_WAVWRITER: log("[FMOD] Output Type: WAVWRITER"); break;
			case FMOD_OUTPUTTYPE_NOSOUND_NRT: log("[FMOD] Output Type: NOSOUND_NRT"); break;
			case FMOD_OUTPUTTYPE_WAVWRITER_NRT: log("[FMOD] Output Type: WAVWRITER_NRT"); break;
			case FMOD_OUTPUTTYPE_DSOUND: log("[FMOD] Output Type: DSOUND"); break;
			case FMOD_OUTPUTTYPE_WINMM: log("[FMOD] Output Type: WINMM"); break;
			case FMOD_OUTPUTTYPE_WASAPI: log("[FMOD] Output Type: WASAPI"); break;
			case FMOD_OUTPUTTYPE_ASIO: log("[FMOD] Output Type: ASIO"); break;
			case FMOD_OUTPUTTYPE_PULSEAUDIO: log("[FMOD] Output Type: PULSEAUDIO"); break;
			case FMOD_OUTPUTTYPE_ALSA: log("[FMOD] Output Type: ALSA"); break;
			case FMOD_OUTPUTTYPE_COREAUDIO: log("[FMOD] Output Type: COREAUDIO"); break;
			case FMOD_OUTPUTTYPE_XAUDIO: log("[FMOD] Output Type: XAUDIO"); break;
			case FMOD_OUTPUTTYPE_PS3: log("[FMOD] Output Type: PS3"); break;
			case FMOD_OUTPUTTYPE_AUDIOTRACK: log("[FMOD] Output Type: AUDIOTRACK"); break;
			case FMOD_OUTPUTTYPE_OPENSL: log("[FMOD] Output Type: OPENSL"); break;
			case FMOD_OUTPUTTYPE_WIIU: log("[FMOD] Output Type: WIIU"); break;
			case FMOD_OUTPUTTYPE_AUDIOOUT: log("[FMOD] Output Type: AUDIOOUT"); break;
			case FMOD_OUTPUTTYPE_AUDIO3D: log("[FMOD] Output Type: AUDIO3D"); break;
			case FMOD_OUTPUTTYPE_ATMOS: log("[FMOD] Output Type: ATMOS"); break;
			case FMOD_OUTPUTTYPE_WEBAUDIO: log("[FMOD] Output Type: WEBAUDIO"); break;
			case FMOD_OUTPUTTYPE_NNAUDIO: log("[FMOD] Output Type: NNAUDIO"); break;
			case FMOD_OUTPUTTYPE_MAX: log("[FMOD] Output Type: MAX"); break;
			default: log("[FMOD] Output Type: ???");
			}

			int driverId;
			fmodSystem->getDriver(&driverId);
			char name[256];
			int systemRate;
			int speakerChannels;
			fmodSystem->getDriverInfo(driverId, name, 255, 0, &systemRate, 0, &speakerChannels);
			log("[FMOD] Device Name: " + std::string(name));
			log("[FMOD] Device Sample Rate: " + std::to_string(systemRate));
			log("[FMOD] Device Channels: " + std::to_string(speakerChannels));

			unsigned bufferLen;
			int buffers;
			fmodSystem->getDSPBufferSize(&bufferLen, &buffers);
			log("[FMOD] DST Buffers: " + std::to_string(bufferLen) + ", " + std::to_string(buffers));
		}
	}

	Sound::~Sound()
	{
		fmodSystem->release();
	}

	Sound& Sound::getInstance()
	{
		static Sound _inst;
		return _inst;
	}

	int Sound::_loadKeySample(std::string path, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, size_t index)
	{
		FMOD_RESULT r =
			fmodSystem->createSound(path.c_str(), mode, exinfo, &keySamples[index]);
#if _DEBUG
		if (r != FMOD_OK)
			log("ERROR: Loading Sample (" + path + ") Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r), LOGS_Minimum);
#endif
		return r;
	}
	int Sound::loadKeySample(std::string path, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, size_t index)
	{
		return getInstance()._loadKeySample(path, mode, exinfo, index);
	}

	void Sound::_playKeySample(size_t count, size_t index[])
	{
		for (size_t i = 0; i < count; i++)
		{
			FMOD_RESULT r = FMOD_OK;
			if (keySamples[index[i]] != nullptr)
				r = fmodSystem->playSound(keySamples[index[i]], 0, false, 0);
#if _DEBUG
			if (r != FMOD_OK)
				log("ERROR: Playing Sample Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r), LOGS_Minimum);
#endif
		}
	}
	void Sound::playKeySample(size_t count, size_t index[])
	{
		getInstance()._playKeySample(count, index);
	}

	void Sound::_freeKeySamples()
	{
		for (auto& s : keySamples)
			if (s != nullptr)
			{
				s->release();
				s = nullptr;
			}
	}
	void Sound::freeKeySamples()
	{
		getInstance()._freeKeySamples();
	}

	int Sound::_update()
	{
		FMOD_RESULT r = fmodSystem->update();
#if _DEBUG
		if (r != FMOD_OK)
			log("ERROR: Sound System Update Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r), LOGS_Minimum);
#endif
		return r;
	}
	int Sound::update()
	{
		return getInstance()._update();
	}

}