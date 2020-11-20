#include "SoundSystem.hpp"

static map<string, Mix_Music*> music = map<string, Mix_Music*>();
static map<string, Mix_Chunk*> sounds = map<string, Mix_Chunk*>();

void SoundSystem::PlayMusic(const string& _musicName, int _loops) {
	if (music[_musicName] != nullptr) {
		Mix_PlayMusic(music[_musicName], _loops);
	}
}

void SoundSystem::PlaySound(const string& _soundName, int _loops, int _channel) {
	if (sounds[_soundName] != nullptr) {
		Mix_PlayChannel(_channel, sounds[_soundName], _loops);
		// TODO -- Add position stuff to change the volume of the sound and all that noise
	}
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

void SoundSystem::LoadMusic(const string& _fileName, const string& _musicName) {
	string filepath = "Resources/Sounds/" + _fileName + ".mp3";
	if (music[_musicName] == nullptr) { // Only load a piece of music that doesn't exist
		music[_musicName] = Mix_LoadMUS(filepath.c_str());
		if (music[_musicName] == nullptr) { // ERROR -- The sound didn't load properly
			DEBUG_ERROR("The music mp3 found at " + filepath + " couldn't be loaded.\nError: " + SDL_GetError());
		}
	}
}

void SoundSystem::LoadSound(const string& _fileName, const string& _soundName) {
	string filepath = "Resources/Sounds/" + _fileName + ".wav";
	if (sounds[_soundName] == nullptr) { // Only load a sound in that doesn't exist
		sounds[_soundName] = Mix_LoadWAV(filepath.c_str());
		if (sounds[_soundName] == nullptr) { // ERROR -- The sound didn't load properly
			DEBUG_ERROR("The sound found at " + filepath + " couldn't be loaded.\nError: " + SDL_GetError());
		}
	}
}

void SoundSystem::UnloadMusic(const string& _musicName) {
	if (music[_musicName] != nullptr) { // Ensures only loaded music get erased and not non-existing ones
		music.erase(_musicName);
	}
}

void SoundSystem::UnloadSound(const string& _soundName) {
	if (sounds[_soundName] != nullptr) { // Ensures only loaded sounds get erased and not non-existing ones
		sounds.erase(_soundName);
	}
}