#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "LevelManager.hpp"
#include "../Objects/ObjectFactory.hpp"
#include "../General/Serialization.hpp"
#include "../Rendering/RenderScene.hpp"

class Level {

	// objects
	ObjectFactory* objFactory;

	// rendering
	RenderScene* renderScene;

public:

	Level(FileIndex* objectIndex_);
	~Level();

	// events
	void Init(const json& data);
	void Update();
	void LateUpdate();
	void Draw();
	void Clean();
	void Exit();

	// getters
	RenderScene* GetRenderScene() const { return renderScene; }
	ObjectFactory* GetFactory() const { return objFactory; }

private:

	// loading functions
	void LoadObjects(const json& data);

};

#endif // !_CORE_LEVEL_HPP