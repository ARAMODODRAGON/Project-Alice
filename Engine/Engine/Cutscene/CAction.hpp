#ifndef C_ACTION_HPP
#define C_ACTION_HPP

#include "../Common.hpp"
#include "CActor.hpp"
#include "../Battle/BattleAlogrithm.hpp"


class CAction
{
public:
	CAction();
	virtual ~CAction();

	virtual void Start() = 0;
	virtual bool Update() = 0;

	RTTR_ENABLE()RTTR_REGISTRATION_FRIEND
};
#endif

