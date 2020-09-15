#ifndef _CORE_LEVEL_HPP
#define _CORE_LEVEL_HPP
#include "LevelManager.hpp"
#include "../Objects/ObjectFactory.hpp"
#include "../General/Serialization.hpp"
#include "../Rendering/RenderScene.hpp"

class Level {

	// levelmanager that this is attached to
	LevelManager* levelManager;

	// objects
	ObjectFactory* objFactory;

	// rendering
	RenderScene* renderScene;

public:

	Level(LevelManager* levelManager_, FileIndex* objectIndex_, ContentHandler* content_);
	~Level();

	// events
	void LoadData(const json& data);
	void Update();
	void Draw();

	// getters
	RenderScene* GetRenderScene() const { return renderScene; }
	ObjectFactory* GetFactory() const { return objFactory; }

private:

	// loading functions
	void LoadObjects(const json& data);

};

#endif // !_CORE_LEVEL_HPP