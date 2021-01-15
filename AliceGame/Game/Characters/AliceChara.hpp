#ifndef ALICE_CHARACTERS_ALICECHARA_HPP
#define ALICE_CHARACTERS_ALICECHARA_HPP
#include "Character.hpp"

class AliceChara final : public Character {
public:

	AliceChara();
	~AliceChara();

	virtual void Start(ALC::Entity self);
	virtual void Update(ALC::Entity self, ALC::Timestep ts);
	virtual void LateUpdate(ALC::Entity self, ALC::Timestep ts);

private:
};

#endif // !ALICE_CHARACTERS_ALICECHARA_HPP