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
	bool isComplete()override;

	RTTR_ENABLE(BTPhase) RTTR_REGISTRATION_FRIEND
};
#endif // !TEST_PHASE_1_HPP
