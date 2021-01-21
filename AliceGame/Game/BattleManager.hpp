#ifndef ALICE_BATTLEMANAGER_HPP
#define ALICE_BATTLEMANAGER_HPP
#include <ALC\General.hpp>
#include <ALC\Bullets.hpp>

class Character;

enum class CharaType : ALC::uint8 {
	None, Alice, Rui, Eden
};

// collision with players
#define BTL_PLAYERMASK	(1 << 0)
// collisions with enemies
#define BTL_ENEMYMASK	(1 << 1)

class BattleManager final {
	ALC_NON_CONSTRUCTABLE(BattleManager);
public:

	static ALC::vec2 PrefferedResolution() { return ALC::vec2(1280.0f, 720.0f); }

	static ALC::rect GetLevelBounds() { return s_levelBounds; }
	static void SetLevelBounds(const ALC::rect& levelBounds) { s_levelBounds = levelBounds; }

	static CharaType GetCharacter() { return s_type; }
	static void SetCharcter(const CharaType type) { s_type = type; }

	// initializes this entity as whatever CharaType is stored in the battlemanager
	// returns null if entity is invalid or CharaType is None
	static Character* InitAsCharacter(ALC::Entity entity);

private:
	static ALC::rect s_levelBounds;
	static CharaType s_type;
};

#endif // !ALICE_BATTLEMANAGER_HPP