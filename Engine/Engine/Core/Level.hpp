#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "LevelManager.hpp"
#include "../Objects/ObjectFactory.hpp"
#include "../General/Serialization.hpp"
#include "../Rendering/RenderScene.hpp"

class Level {

public:

	Level() { }
	virtual ~Level() { }

	// events
	virtual void Init() { }
	virtual void Update() { }
	virtual void LateUpdate() { }
	virtual void Exit() { }

};

#endif // !_CORE_LEVEL_HPP