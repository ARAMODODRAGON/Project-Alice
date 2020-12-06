#include "AudioSourceComponent.hpp"

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