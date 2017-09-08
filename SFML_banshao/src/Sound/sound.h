#pragma once

#include <utility>
#include <array>
#include <string>
#include <fmod.hpp>

// This game uses FMOD Low Level API to play Sounds as we don't use FMOD Studio,
// since the audio part of SFML is kinda not enough (mostly low-latency support).

namespace game
{
	class Sound
	{
	private:
		FMOD::System *fmodSystem = nullptr;
		std::pair<int, std::string> initResult;

	public:
		Sound();
		~Sound();

	private:
		static const size_t KEYSAMPLES = 36 * 36;
		std::array<FMOD::Sound*, KEYSAMPLES> keySamples{};

	public:
		int loadKeySample(std::string path, size_t index);
		void playKeySample(size_t count, size_t index[]);
		void freeKeySamples();
		int update();

	private:
		static const size_t ETCSAMPLES = 32;
		std::array<FMOD::Sound*, ETCSAMPLES> etcSamples{};

	public:
		int loadSample(std::string path, size_t index, bool isStream = false, bool loop = false);
		void playSample(size_t index);
		void freeSamples();

	};
}
