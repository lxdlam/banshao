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
		Sound();
		~Sound();
		FMOD::System *fmodSystem = nullptr;
		std::pair<int, std::string> initResult;
	public:
		static Sound& getInstance();

	private:
		std::array<FMOD::Sound*, 36 * 36> keySamples{};
		int _loadKeySample(std::string path, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, size_t index);
		void _playKeySample(size_t count, size_t index[]);
		void _freeKeySamples();
		int _update();
	public:
		static int loadKeySample(std::string path, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, size_t index);
		static void playKeySample(size_t count, size_t index[]);
		static void freeKeySamples();
		static int update();

	};
}
