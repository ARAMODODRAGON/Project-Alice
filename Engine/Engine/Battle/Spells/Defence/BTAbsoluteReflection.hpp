#ifndef DEF_ABSOLUTE_REFLECTION_HPP
#define DEF_ABSOLUTE_REFLECTION_HPP

#include "BTDefenceSpell.hpp"

class BTAbsoluteReflection : public BTDefenceSpell {
public:
	BTAbsoluteReflection();
	~BTAbsoluteReflection();

	void Use() override;

	RTTR_ENABLE(BTDefenceSpell) RTTR_REGISTRATION_FRIEND
};

#endif