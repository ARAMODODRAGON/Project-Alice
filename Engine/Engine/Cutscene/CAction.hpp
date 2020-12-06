#ifndef C_ACTION_HPP
#define C_ACTION_HPP

#include "../Common.hpp"
#include "CActor.hpp"
#include "../Battle/BattleAlogrithm.hpp"


class CAction
{
public:
	CAction() { }
	virtual ~CAction() = 0 { }

	virtual void Start() = 0;
	virtual bool Update() = 0;
	virtual bool isRunning() = 0;

};
#endif

