#ifndef ALICE_CHARACTERS_CHARACTER_HPP
#define ALICE_CHARACTERS_CHARACTER_HPP
#include <ALC\Bullets.hpp>

class Character : public ALC::ShooterBehavior {
public:

	Character();
	virtual ~Character() = 0;

private:

};

#endif // !ALICE_CHARACTERS_CHARACTER_HPP