#ifndef SCRIPTS_DEBUG_MENU_HPP
#define SCRIPTS_DEBUG_MENU_HPP
#include <Engine/Common.hpp>

#define DEBUG_MENU_FPS_AVERAGE_FRAME_COUNT 10

class DebugMenu : public Object {

	bool drawUI; // toggled using 'D' key
	UIRenderer* ui;

	array<double, DEBUG_MENU_FPS_AVERAGE_FRAME_COUNT> multiFrameFPS;
	uint32 frameIndex;

public:

	DebugMenu();
	~DebugMenu();

	// events
	virtual void Start() override;
	virtual void LateUpdate() override;

};

#endif // !SCRIPTS_DEBUG_MENU_HPP