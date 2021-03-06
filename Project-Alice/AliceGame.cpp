#include "AliceGame.hpp"
#include "Scripts/Overworld/SpellInventory.hpp"
#include "Scripts/Test/Tiles/TestTile.hpp"
#include <Engine/General/SaveSystem.hpp>				 
#include <Engine/Content/SoundSystem.hpp>
#include <Engine/General/SaveSystem.hpp>
#include <Engine/Input/Keyboard.hpp>
#include <Engine/Battle/BattleManager.hpp>
#include <Engine/Physics/PhysicsScene.hpp>
#include <Engine/Cutscene/CutsceneManager.hpp>
#include <Engine/Battle/TileMap/TileMap.hpp>
#include "Scripts\Levels\BattleDemo\BattleDemoLevel.hpp"

//#include <Engine/Battle/Spells/SpellMacros.hpp>
#include <random>
#include <chrono>
#include <iostream>

#define DEFAULT_LEVEL "battle_demo"

AliceGame::AliceGame()
	: Game()
	, quitTimer(0)
	, max_quit_time(15) { }

AliceGame::~AliceGame() { }

bool AliceGame::Init() {

	// FOR TESTING READING/WRITING DATA //

	SaveSystem::LoadData("Save");

	if (!Game::Init()) return false;

	// FOR TESTING SOUND SYSTEM //

	SoundSystem::LoadMusic("Another Solo", "Test");
	//SoundSystem::PlayMusic("Test");

	SoundSystem::LoadSound("MeepMeep", "Test");
	//SoundSystem::PlaySound("Test", -1);

	// initialize the singletons
	ContentHandler::Init("Resources/Textures/Textures.index", "Resources/Shaders/Shaders.index", "Resources/Fonts/Fonts.index");
	RenderScene::Init();
	ObjectFactory::Init("Resources/Objects/Objects.index");
	PhysicsScene::Init();
	BattleManager::Init("Resources/EnemyPhases/EnemyPhases.index");
	LevelManager::Init<BattleDemoLevel>();
	CutsceneManager::Init("Resources/Cutscenes/Cutscene.index");

	// FOR TESTING SPELL INVENTORY //

	SpellList::InitSpellData("Resources/Spells/Spells.json");
	SpellInventory::InitData();

	//SpellInventory::AddSpell("Boomerang");
	//SpellInventory::AddSpell("Wind Slash");
	//SpellInventory::AddSpell("Homing Lasers");
	//SpellInventory::AddSpell("Absolute Barrier");
	//SpellInventory::AddSpell("Absolute Reflection");
	//SpellInventory::AddSpell("Healing");

	//DEBUG_LOG("No. of Attack Spells: " + VTOS(SpellInventory::GetSpellCount(SpellType::Attack)));
	//DEBUG_LOG("No. of Defence Spells: " + VTOS(SpellInventory::GetSpellCount(SpellType::Defence)));

	//SpellInventory::EquipAtkSpell("Boomerang", 0);
	//SpellInventory::EquipAtkSpell("Wind Slash", 1);
	//SpellInventory::EquipAtkSpell("Wind Slash", 2);

	//SpellInventory::EquipDefSpell("Absolute Barrier");

	//for (uint32 i = 0; i < MAX_EQUIPPED_SPELLS; i++) {
	//	DEBUG_LOG("Atk Skill Slot " + VTOS(i + 1) + ": " + VTOS(SpellInventory::GetEquippedAtkSpell(i)));
	//}
	//DEBUG_LOG("Def Skill: " + VTOS(SpellInventory::GetEquippedDefSpell()));

	// FOR TESTING SPELL INVENTORY //


	return true;
}

void AliceGame::Update() {

	PollEvents();

	if (Keyboard::GetKey(KeyCode::Escape)) {
		++quitTimer;
		if (quitTimer > max_quit_time) Quit();
	} else {
		quitTimer = 0;
	}

	// update 
	BattleManager::Update(1.0f);
	LevelManager::Update();
	ObjectFactory::Update();
	//CutsceneManager::Update();

	// do physics
	PhysicsScene::Step();

}

void AliceGame::Draw() {

	// late update
	LevelManager::LateUpdate();
	ObjectFactory::LateUpdate();


	// draw 
	GetWindow()->ClearScreen(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	RenderScene::Draw();

	// cleanup
	ObjectFactory::Cleanup();
	LevelManager::Cleanup();
	ContentHandler::Clean();

	// wait for end of frame
	GetTimer()->WaitForEndOfFrame();
	GetWindow()->SwapBuffers();


}

bool AliceGame::Exit() {
	if (!Game::Exit()) return false;

	// Exit the singletons
	LevelManager::Exit();
	ObjectFactory::Exit();
	PhysicsScene::Exit();
	RenderScene::Exit();
	ContentHandler::Exit();
	//CutsceneManager::Exit();

	return true;
}

static void LoadObjects(const json& data) {
	DEBUG_LOG("Does nothing");
}

void AliceGame::LevelLoad(Level* level, const json& data) {
	// destroy all objects
	ObjectFactory::Clear();

	// load the level data
	if (data.contains("objects"))
		LoadObjects(data["objects"]);

	// TODO: remove this in the future
	if (data.contains("HasPathfinding") && data["HasPathfinding"] == true) {

		TileMap::Init();
		float initPosX = -99.0f;
		vec2 currntPosition(vec2(initPosX, 40.0f));
		//int32 tmpArr[ROW][COL];


		for (int r = 0; r < ROW; ++r) {
			for (int c = 0; c < COL; ++c) {

				TestTile* tile = new TestTile();

				tmpArr[r][c] = TileMap::GetTileElement(r, c);

				if (tmpArr[r][c] == 0) {
					tile->SetIsWalkable(true);
				} else {
					tile->SetIsWalkable(false);
				}

				tile->Start();
				tile->SetPosition(currntPosition);
				TileMap::AddTile(tile);
				currntPosition.x += tile->GetHeightAndWidth().x;

				if (currntPosition.x >= 96) {
					currntPosition.x = initPosX;
					currntPosition.y -= tile->GetHeightAndWidth().y;
				}
			}
		}

		if (bta::PathTo(std::make_pair(0, 0), std::make_pair(3, COL - 1), tmpArr)) {
			DEBUG_LOG("Path found ");


		} else {
			DEBUG_LOG("No path found");
		}
	}

}

int main(int argc, char* argv[]) {

	// set random value using chrono
	srand(std::chrono::steady_clock::now().time_since_epoch().count());

	Game* game = new AliceGame();
	game->Run();
	delete game;



	return 0;
}