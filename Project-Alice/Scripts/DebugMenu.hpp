#ifndef SCRIPTS_DEBUG_MENU_HPP
#define SCRIPTS_DEBUG_MENU_HPP
#include <Engine/Common.hpp>

class DebugMenu : public Object {

	bool drawUI; // toggled using 'D' key
	UIRenderer* ui;

public:

	DebugMenu();
	~DebugMenu();

	// events
	virtual void Start() override;
	virtual void LateUpdate() override;


	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif // !SCRIPTS_DEBUG_MENU_HPP