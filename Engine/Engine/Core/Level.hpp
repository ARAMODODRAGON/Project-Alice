#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "LevelManager.hpp"
#include "../Objects/ObjectFactory.hpp"
#include "../General/Serialization.hpp"
#include "../Rendering/RenderScene.hpp"

class Level {

public:

	Level() { }
	virtual ~Level() = 0 { }

	// events
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void LateUpdate() = 0;
	virtual void Exit() = 0;

	RTTR_ENABLE() RTTR_REGISTRATION_FRIEND
};

#endif // !_CORE_LEVEL_HPP