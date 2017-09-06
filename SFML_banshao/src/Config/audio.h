#pragma once

namespace game::Config
{
	class audio
	{
	private:
		audio() = default;
		~audio() = default;
	public:
		static audio& getInstance();
		audio(audio const&) = delete;
		void operator=(audio const&) = delete;
		int setDefaults();
		int loadConfig();
		
	private:
		unsigned bufferLength = 256;
		int bufferCount = 2;

	public:
		unsigned getBufferLength();
		int getBufferCount();

		// TODO getters setters
	};
}