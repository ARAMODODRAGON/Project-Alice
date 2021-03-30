#include "BattleManager.hpp"
#include "Characters\AliceChara.hpp"
#include "Characters\RuiChara.hpp"
#include "Enemies\Enemy.hpp"

bool BattleManager::s_isBattleActive = false;
ALC::rect BattleManager::s_levelBounds;
CharaType BattleManager::s_type = CharaType::None;
Enemy* BattleManager::s_enemy = nullptr;
Character* BattleManager::s_player = nullptr;

void BattleManager::AddEnemy(Enemy* enemy) {
	s_enemy = enemy;
}

Enemy* BattleManager::GetEnemy() {
	return s_enemy;
}

void BattleManager::RemoveEnemy(Enemy* enemy) {
	if (s_enemy == enemy) s_enemy = nullptr;
}

Character* BattleManager::InitAsCharacter(ALC::Entity entity) {
	// return if invalid entity 
	if (!entity) return nullptr;

	s_player = nullptr;
	switch (s_type) {
		case CharaType::Alice:
			s_player = entity.AddBehavior<AliceChara>();
			break;
		case CharaType::Rui:
			s_player = entity.AddBehavior<RuiChara>();
			break;
		case CharaType::Eden:
			ALC_DEBUG_ERROR("Character type 'Eden' is not supported");
			break;
		default: break;
	}
	return s_player;
}

Character* BattleManager::GetCurrentCharacter() {
	return s_player;
}

void BattleManager::ToggleBattle() {
	// start
	if (!s_isBattleActive) {
		// calls begin battle
		if (s_player && s_enemy) {
			s_enemy->BattleToggle();
			s_player->BattleToggle();
			s_isBattleActive = true;
		}
		// failed
		else {
			ALC_DEBUG_ERROR("Could not start battle without both enemy and player");
		}
	}
	// end
	else {
		s_player->BattleToggle();
		s_isBattleActive = false;
	}
}
