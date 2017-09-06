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
			case FMOD_OUTPUTTYPE_AUTODETECT: log("[FMOD] Type: FMOD_OUTPUTTYPE_AUTODETECT"); break;
			case FMOD_OUTPUTTYPE_UNKNOWN: log("[FMOD] Type: FMOD_OUTPUTTYPE_UNKNOWN"); break;
			case FMOD_OUTPUTTYPE_NOSOUND: log("[FMOD] Type: FMOD_OUTPUTTYPE_NOSOUND"); break;
			case FMOD_OUTPUTTYPE_WAVWRITER: log("[FMOD] Type: FMOD_OUTPUTTYPE_WAVWRITER"); break;
			case FMOD_OUTPUTTYPE_NOSOUND_NRT: log("[FMOD] Type: FMOD_OUTPUTTYPE_NOSOUND_NRT"); break;
			case FMOD_OUTPUTTYPE_WAVWRITER_NRT: log("[FMOD] Type: FMOD_OUTPUTTYPE_WAVWRITER_NRT"); break;
			case FMOD_OUTPUTTYPE_DSOUND: log("[FMOD] Type: FMOD_OUTPUTTYPE_DSOUND"); break;
			case FMOD_OUTPUTTYPE_WINMM: log("[FMOD] Type: FMOD_OUTPUTTYPE_WINMM"); break;
			case FMOD_OUTPUTTYPE_WASAPI: log("[FMOD] Type: FMOD_OUTPUTTYPE_WASAPI"); break;
			case FMOD_OUTPUTTYPE_ASIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_ASIO"); break;
			case FMOD_OUTPUTTYPE_PULSEAUDIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_PULSEAUDIO"); break;
			case FMOD_OUTPUTTYPE_ALSA: log("[FMOD] Type: FMOD_OUTPUTTYPE_ALSA"); break;
			case FMOD_OUTPUTTYPE_COREAUDIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_COREAUDIO"); break;
			case FMOD_OUTPUTTYPE_XAUDIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_XAUDIO"); break;
			case FMOD_OUTPUTTYPE_PS3: log("[FMOD] Type: FMOD_OUTPUTTYPE_PS3"); break;
			case FMOD_OUTPUTTYPE_AUDIOTRACK: log("[FMOD] Type: FMOD_OUTPUTTYPE_AUDIOTRACK"); break;
			case FMOD_OUTPUTTYPE_OPENSL: log("[FMOD] Type: FMOD_OUTPUTTYPE_OPENSL"); break;
			case FMOD_OUTPUTTYPE_WIIU: log("[FMOD] Type: FMOD_OUTPUTTYPE_WIIU"); break;
			case FMOD_OUTPUTTYPE_AUDIOOUT: log("[FMOD] Type: FMOD_OUTPUTTYPE_AUDIOOUT"); break;
			case FMOD_OUTPUTTYPE_AUDIO3D: log("[FMOD] Type: FMOD_OUTPUTTYPE_AUDIO3D"); break;
			case FMOD_OUTPUTTYPE_ATMOS: log("[FMOD] Type: FMOD_OUTPUTTYPE_ATMOS"); break;
			case FMOD_OUTPUTTYPE_WEBAUDIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_WEBAUDIO"); break;
			case FMOD_OUTPUTTYPE_NNAUDIO: log("[FMOD] Type: FMOD_OUTPUTTYPE_NNAUDIO"); break;
			case FMOD_OUTPUTTYPE_MAX: log("[FMOD] Type: FMOD_OUTPUTTYPE_MAX"); break;
			default: log("[FMOD] Type: ???");
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
		FMOD_RESULT r =
			fmodSystem->update();
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