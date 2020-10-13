#ifndef TEST_PHASE_HPP
#define TEST_PHASE_HPP

#include"Engine/Battle/BTPhase.hpp"

class TestPhase : public BTPhase
{
	TestPhase();
	virtual ~TestPhase();

	void StartPhase()override;
	void UpdatePhase()override;

};
#endif

