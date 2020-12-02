#include "AudioSourceComponent.hpp"

RTTR_REGISTRATION{
	registration::class_<AudioSourceComponent>("ShootComponent")
		.public_object_constructor;
}

AudioSourceComponent::AudioSourceComponent() 
	: soundName("") 
{}

AudioSourceComponent::~AudioSourceComponent() {
}

void AudioSourceComponent::PlaySound(int _loops, int _channel) {
	SoundSystem::PlaySound(soundName, _loops, _channel);
}

void AudioSourceComponent::SetSound(const string& _soundName) {
	soundName = _soundName;
}