#include "sound.h"
#include "../game/configManager.h"
#include "../defs.h"
#include "../utils.h"
#include <fmod_errors.h>
using namespace game::defs::config;

namespace game
{
	Sound::Sound()
	{
		FMOD_RESULT r;

		r = FMOD::System_Create(&fmodSystem);      // Create the main system object.
		if (r != FMOD_OK)
		{
			LOG(ERROR) << "Create FMOD System Failed: " << FMOD_ErrorString(r);
			return;
		}

		fmodSystem->setDSPBufferSize(
			conf().audio.get<unsigned>(aud_bufLen),
			conf().audio.get<int>(aud_bufCount)
		);

		r = fmodSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
		if (r != FMOD_OK)
		{
			LOG(ERROR) << "FMOD System Initialize Failed: " << FMOD_ErrorString(r);
			return;
		}

		initResult = { r, FMOD_ErrorString(r) };

		if (r == FMOD_OK)
		{
			LOG(DEBUG) << "FMOD System Initialize Finished.";

			FMOD_OUTPUTTYPE outputtype;
			fmodSystem->getOutput(&outputtype);
			switch (outputtype)
			{
			case FMOD_OUTPUTTYPE_AUTODETECT: LOG(INFO) << "[FMOD] Output Type: AUTODETECT"; break;
			case FMOD_OUTPUTTYPE_UNKNOWN: LOG(INFO) << "[FMOD] Output Type: UNKNOWN"; break;
			case FMOD_OUTPUTTYPE_NOSOUND: LOG(INFO) << "[FMOD] Output Type: NOSOUND"; break;
			case FMOD_OUTPUTTYPE_WAVWRITER: LOG(INFO) << "[FMOD] Output Type: WAVWRITER"; break;
			case FMOD_OUTPUTTYPE_NOSOUND_NRT: LOG(INFO) << "[FMOD] Output Type: NOSOUND_NRT"; break;
			case FMOD_OUTPUTTYPE_WAVWRITER_NRT: LOG(INFO) << "[FMOD] Output Type: WAVWRITER_NRT"; break;
			case FMOD_OUTPUTTYPE_DSOUND: LOG(INFO) << "[FMOD] Output Type: DSOUND"; break;
			case FMOD_OUTPUTTYPE_WINMM: LOG(INFO) << "[FMOD] Output Type: WINMM"; break;
			case FMOD_OUTPUTTYPE_WASAPI: LOG(INFO) << "[FMOD] Output Type: WASAPI"; break;
			case FMOD_OUTPUTTYPE_ASIO: LOG(INFO) << "[FMOD] Output Type: ASIO"; break;
			case FMOD_OUTPUTTYPE_PULSEAUDIO: LOG(INFO) << "[FMOD] Output Type: PULSEAUDIO"; break;
			case FMOD_OUTPUTTYPE_ALSA: LOG(INFO) << "[FMOD] Output Type: ALSA"; break;
			case FMOD_OUTPUTTYPE_COREAUDIO: LOG(INFO) << "[FMOD] Output Type: COREAUDIO"; break;
			case FMOD_OUTPUTTYPE_XAUDIO: LOG(INFO) << "[FMOD] Output Type: XAUDIO"; break;
			case FMOD_OUTPUTTYPE_PS3: LOG(INFO) << "[FMOD] Output Type: PS3"; break;
			case FMOD_OUTPUTTYPE_AUDIOTRACK: LOG(INFO) << "[FMOD] Output Type: AUDIOTRACK"; break;
			case FMOD_OUTPUTTYPE_OPENSL: LOG(INFO) << "[FMOD] Output Type: OPENSL"; break;
			case FMOD_OUTPUTTYPE_WIIU: LOG(INFO) << "[FMOD] Output Type: WIIU"; break;
			case FMOD_OUTPUTTYPE_AUDIOOUT: LOG(INFO) << "[FMOD] Output Type: AUDIOOUT"; break;
			case FMOD_OUTPUTTYPE_AUDIO3D: LOG(INFO) << "[FMOD] Output Type: AUDIO3D"; break;
			case FMOD_OUTPUTTYPE_ATMOS: LOG(INFO) << "[FMOD] Output Type: ATMOS"; break;
			case FMOD_OUTPUTTYPE_WEBAUDIO: LOG(INFO) << "[FMOD] Output Type: WEBAUDIO"; break;
			case FMOD_OUTPUTTYPE_NNAUDIO: LOG(INFO) << "[FMOD] Output Type: NNAUDIO"; break;
			case FMOD_OUTPUTTYPE_MAX: LOG(INFO) << "[FMOD] Output Type: MAX"; break;
			default: LOG(INFO) << "[FMOD] Output Type: ???";
			}

			int driverId;
			fmodSystem->getDriver(&driverId);
			char name[256];
			int systemRate;
			int speakerChannels;
			fmodSystem->getDriverInfo(driverId, name, 255, 0, &systemRate, 0, &speakerChannels);
			LOG(INFO) << "[FMOD] Device Name: " << name;
			LOG(INFO) << "[FMOD] Device Sample Rate: " << systemRate;
			LOG(INFO) << "[FMOD] Device Channels: " << speakerChannels;

			unsigned bufferLen;
			int buffers;
			fmodSystem->getDSPBufferSize(&bufferLen, &buffers);
			LOG(INFO) << "[FMOD] DST Buffers: " << bufferLen << ", " << buffers;
		}
	}

	Sound::~Sound()
	{
		fmodSystem->release();
		LOG(DEBUG) << "FMOD System released.";
	}

	// FIXME COLORS INDENTS VALUABLES
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
			LOG(DEBUG) << "[FMOD] Loading Sample (" + path + ") Error: " << r << ", " << FMOD_ErrorString(r);
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
				LOG(WARNING) << "[FMOD] Playing Sample Error: " << r << ", " << FMOD_ErrorString(r);
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
			LOG(WARNING) << "[FMOD] Loading Sample (" << path << ") Error: " << r << ", " << FMOD_ErrorString(r);
		return r;
	}

	void Sound::playSample(size_t index)
	{
		FMOD_RESULT r = FMOD_OK;
		if (etcSamples[index] != nullptr)
			r = fmodSystem->playSound(etcSamples[index], 0, false, 0);
#if _DEBUG
		if (r != FMOD_OK)
			LOG(WARNING) << "[FMOD] Playing Sample Error: " << r << ", " << FMOD_ErrorString(r);
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
		if (!fmodSystem) return 1;
			FMOD_RESULT r = fmodSystem->update();
#if _DEBUG
		if (r != FMOD_OK)
			LOG(ERROR) << "[FMOD] Sound System Update Error: " << r << ", " << FMOD_ErrorString(r);
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