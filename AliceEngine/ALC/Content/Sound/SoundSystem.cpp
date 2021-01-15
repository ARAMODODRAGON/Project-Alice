#include "SoundSystem.hpp"
#include <SDL.h>
#include <SDL_mixer.h>

namespace ALC {

	static unordered_map<string, Mix_Music*> g_music;
	static unordered_map<string, Mix_Chunk*> g_sounds;

	void SoundSystem::PlayMusic(const string& file, int _loops) {
		auto it = g_music.find(file);
		if (it != g_music.end()) {
			Mix_PlayMusic(it->second, _loops);
			return;
		}
		ALC_DEBUG_ERROR("Could not play music file. It was not found. " + file); 
	}

	void SoundSystem::PlaySound(const string& file, int _loops, int _channel) {
		auto it = g_sounds.find(file);
		if (it != g_sounds.end()) {
			Mix_PlayChannel(_channel, it->second, _loops);
		}
		ALC_DEBUG_ERROR("Could not play sound file. It was not found. " + file);
	}

	void SoundSystem::PauseMusic() {
		if (Mix_PlayingMusic() != 0) {
			Mix_PauseMusic();
		}
	}

	void SoundSystem::ResumeMusic() {
		if (Mix_PausedMusic() != 0) {
			Mix_ResumeMusic();
		}
	}

	bool SoundSystem::LoadMusic(const string& file) {
		if (g_music.find(file) != g_music.end()) {
			return true;
		}

		Mix_Music* music = Mix_LoadMUS(file.c_str());
		if (music != nullptr) {
			g_music.emplace(file, music);
			return true;
		}

		ALC_DEBUG_ERROR("Could not load music file at " + file);
		return false;
	}

	bool SoundSystem::LoadSound(const string& file) {
		if (g_sounds.find(file) != g_sounds.end()) {
			return true;
		}

		Mix_Chunk* sound = Mix_LoadWAV(file.c_str());
		if (sound != nullptr) {
			g_sounds.emplace(file, sound);
			return true;
		}

		ALC_DEBUG_ERROR("Could not load sound file at " + file);
		return false;
	}

	void SoundSystem::UnloadMusic(const string& file) {
		auto mus = g_music.find(file);
		if (mus != g_music.end()) {
			Mix_FreeMusic(mus->second);
			g_music.erase(mus);
		}
	}

	void SoundSystem::UnloadSound(const string& file) {
		auto sou = g_sounds.find(file);
		if (sou != g_sounds.end()) {
			Mix_FreeChunk(sou->second);
			g_sounds.erase(sou);
		}
	}

}