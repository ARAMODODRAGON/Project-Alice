#ifndef COMPONENT_AUDIO_SOURCE_HPP
#define COMPONENT_AUDIO_SOURCE_HPP

#include "SoundSystem.hpp"
#include "../Objects/Component.hpp"

class AudioSourceComponent : public Component {
	string soundName;
	// TODO -- Add position stuff here for positional volume
public:
	AudioSourceComponent();
	~AudioSourceComponent();

	void PlaySound(int _loops = 0, int _channel = 0);
	void SetSound(const string& _soundName);

	RTTR_ENABLE(Component) RTTR_REGISTRATION_FRIEND
};

#endif