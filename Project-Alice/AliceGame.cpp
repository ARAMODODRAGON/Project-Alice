#include "AliceGame.hpp"
#include <Engine/Keyboard.hpp>

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(60)
	, entityIndex(nullptr)
	, levelIndex(nullptr)
	, scriptableIndex(nullptr)
	, efactory(nullptr) { }

AliceGame::~AliceGame() { }

bool AliceGame::Start() {
	if (!Game::Start()) return false;

	// initialize the indexes
	entityIndex = new ObjectIndex("Objects/Entities");
	levelIndex = new ObjectIndex("Objects/Levels");
	scriptableIndex = new ObjectIndex("Objects/Scriptables");

	//auto ty = type::get<Entity>();
	//if (!ty.is_valid()) {
	//	DEBUG_ERROR("Its not valid!");
	//} else {
	//	DEBUG_LOG("name is: " + ty.get_name());
	//}

	// initialize the factory
	efactory = new EntityFactory(entityIndex); // register the index
	Entity* e = efactory->Make<Entity>("Alice Battle");
	if (e) {
		DEBUG_LOG("Entity name: " + e->GetName());
		DEBUG_LOG("Entity velocity is: {" + std::to_string(e->GetVelocity().x) + ", " + std::to_string(e->GetVelocity().y) + "}");
	}
	return true;
}

void AliceGame::Update() {

	GetSystemEvents()->PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

	efactory->Update();

}

void AliceGame::Draw() {

	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));

	efactory->Draw();

	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();
}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// delete the factory
	if (efactory) delete efactory;
	efactory = nullptr;

	// delete the indexes
	if (entityIndex) delete entityIndex;
	entityIndex = nullptr;
	if (levelIndex) delete levelIndex;
	levelIndex = nullptr;
	if (scriptableIndex) delete scriptableIndex;
	scriptableIndex = nullptr;

	return true;
}




int main(int argc, char* argv[]) {

	Game* game = new AliceGame();
	game->Run();
	delete game;

	return 0;
}