#include "BattleManager.hpp"
#include "Characters\AliceChara.hpp"

ALC::rect BattleManager::s_levelBounds;
CharaType BattleManager::s_type = CharaType::None;

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
