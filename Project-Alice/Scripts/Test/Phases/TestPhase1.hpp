#ifndef TEST_PHASE_1_HPP
#define TEST_PHASE_1_HPP

#include "Engine/Battle/BTPhase.hpp"

class TestPhase1 : public BTPhase
{
	BTAResult result;
public:
	TestPhase1();
	virtual ~TestPhase1();

	void StartPhase()override;
	void UpdatePhase()override;
	bool IsComplete()override;

};
#endif // !TEST_PHASE_1_HPP
