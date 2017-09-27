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
			log("[FMOD] FMOD System Initialize Finished.", LOGS_Core);

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
		log("[FMOD] FMOD System released.", LOGS_Core);
	}

	int Sound::loadKeySample(std::string path, size_t index)
	{
		if (path.empty())
			return FMOD_OK;
		FMOD_RESULT r =
			fmodSystem->createSound(path.c_str(), FMOD_UNIQUE, 0, &keySamples[index]);
		if (r == FMOD_ERR_FILE_NOTFOUND && path.length() > 4 && path.substr(path.length() - 4) == ".wav")
			r = fmodSystem->createSound(path.replace(path.length() - 4, 4, ".ogg").c_str(), FMOD_UNIQUE, 0, &keySamples[index]);
#if _DEBUG
		if (r != FMOD_OK)
			log("[FMOD] Loading Sample (" + path + ") Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r));
#endif
		return r;
	}

	void Sound::playKeySample(size_t count, size_t index[])
	{
		for (size_t i = 0; i < count; i++)
		{
			FMOD_RESULT r = FMOD_OK;
			if (keySamples[index[i]] != nullptr)
				r = fmodSystem->playSound(keySamples[index[i]], 0, false, 0);
#if _DEBUG
			if (r != FMOD_OK)
				log("ERROR: Playing Sample Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r));
#endif
		}
	}

	void Sound::freeKeySamples()
	{
		for (auto& s : keySamples)
			if (s != nullptr)
			{
				s->release();
				s = nullptr;
			}
	}

	int Sound::loadSample(std::string path,size_t index, bool isStream, bool loop)
	{
		if (etcSamples[index] != nullptr)
			etcSamples[index]->release();

		int flag = FMOD_DEFAULT;
		if (isStream)
			flag |= FMOD_CREATESTREAM;
		if (loop)
			flag |= FMOD_LOOP_NORMAL;
		FMOD_RESULT r =
			fmodSystem->createSound(path.c_str(), flag, 0, &etcSamples[index]);
		
		if (r != FMOD_OK)
			log("[FMOD] Loading Sample (" + path + ") Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r));
		return r;
	}

	void Sound::playSample(size_t index)
	{
		FMOD_RESULT r = FMOD_OK;
		if (etcSamples[index] != nullptr)
			r = fmodSystem->playSound(etcSamples[index], 0, false, 0);
#if _DEBUG
		if (r != FMOD_OK)
			log("[FMOD] Playing Sample Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r));
#endif
	}

	void Sound::freeSamples()
	{
		for (auto& s : etcSamples)
			if (s != nullptr)
			{
				s->release();
				s = nullptr;
			}
	}

	int Sound::update()
	{
		FMOD_RESULT r = fmodSystem->update();
#if _DEBUG
		if (r != FMOD_OK)
			log("[FMOD] Sound System Update Error: " + std::to_string(r) + ", " + FMOD_ErrorString(r));
#endif
		return r;
	}

	int Sound::getChannelsPlaying()
	{
		int c = 0;
		fmodSystem->getChannelsPlaying(&c);
		return c;
	}
}