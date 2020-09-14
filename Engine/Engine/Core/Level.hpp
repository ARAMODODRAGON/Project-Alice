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

	Level(const json& data, LevelManager* levelManager_, FileIndex* objectIndex_ = nullptr, ContentHandler* content_ = nullptr);
	~Level();

	// events
	void Update();
	void Draw();

	// getters
	RenderScene* GetRenderScene() const { return renderScene; }

private:

	// loading functions
	void LoadObjects(const json& data);

};

#endif // !_CORE_LEVEL_HPP