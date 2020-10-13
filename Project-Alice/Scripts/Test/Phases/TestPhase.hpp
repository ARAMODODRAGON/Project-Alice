#ifndef TEST_PHASE_HPP
#define TEST_PHASE_HPP

#include"Engine/Battle/BTPhase.hpp"

class TestPhase : public BTPhase
{
public:
	TestPhase();
	virtual ~TestPhase();

	void StartPhase()override;
	void UpdatePhase()override;
	bool isComplete()override;

	RTTR_ENABLE(BTPhase) RTTR_REGISTRATION_FRIEND

};
#endif

