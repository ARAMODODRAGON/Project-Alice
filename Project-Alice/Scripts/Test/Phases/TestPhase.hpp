#ifndef TEST_PHASE_HPP
#define TEST_PHASE_HPP

#include"Engine/Battle/BTPhase.hpp"

class TestPhase : public BTPhase
{
	int32 timer;
	int32 timerEnd;
	BTAResult result;
public:
	TestPhase();
	virtual ~TestPhase();

	void StartPhase()override;
	void UpdatePhase()override;
	bool IsComplete()override;

};
#endif

