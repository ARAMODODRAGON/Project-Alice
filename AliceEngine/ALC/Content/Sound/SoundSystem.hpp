#ifndef SOUND_SYSTEM_HPP
#define SOUND_SYSTEM_HPP

#include "../../General.hpp"

namespace ALC {

	class SoundSystem {
		ALC_NON_CONSTRUCTABLE(SoundSystem);
	public:
		
		// starts playing the music file,
		// overrides whatever was last playing
		static void PlayMusic(const string& file, int _loops = -1);

		// starts playing the sound file
		static void PlaySound(const string& file, int _loops = 0, int _channel = -1);

		// pauses the currently playing music
		static void PauseMusic();
		
		// plays the current music
		static void ResumeMusic();

		// loads a WAVE, MOD, MIDI, OGG, MP3, or FLAC file
		static bool LoadMusic(const string& file);

		// loads a WAVE, AIFF, RIFF, OGG, or VOC file
		static bool LoadSound(const string& file);

		static void UnloadMusic(const string& file);
		static void UnloadSound(const string& file);
	};

}

#endif