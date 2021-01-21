#include "BattleManager.hpp"
#include "Characters\AliceChara.hpp"

ALC::rect BattleManager::s_levelBounds;
CharaType BattleManager::s_type = CharaType::None;
Enemy* BattleManager::s_enemy = nullptr;

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

	Character* character = nullptr;
	switch (s_type) {
		case CharaType::Alice:
			character = entity.AddBehavior<AliceChara>();
			break;
		case CharaType::Rui:
			ALC_DEBUG_ERROR("Character type 'Rui' is not supported");
			break;
		case CharaType::Eden:
			ALC_DEBUG_ERROR("Character type 'Eden' is not supported");
			break;
		default: break;
	}
	return character;
}
