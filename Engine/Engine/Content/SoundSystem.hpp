#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include "../Common.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

class SoundSystem {
	PRIVATE_SINGLETON(SoundSystem);
public:
	SoundSystem();
	~SoundSystem();

	static void PlayMusic(const string& _musicName, int _loops = -1);
	static void PlaySound(const string& _soundName, int _loops = 0, int _channel = 0);
	static void PauseMusic();
	static void ResumeMusic();

	static void LoadMusic(const string& _fileName, const string& _musicName);
	static void LoadSound(const string& _fileName, const string& _soundName);
	static void UnloadMusic(const string& _musicName);
	static void UnloadSound(const string& _soundName);
};

#endif