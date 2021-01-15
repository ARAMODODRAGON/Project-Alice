#ifndef ALICE_BATTLEMANAGER_HPP
#define ALICE_BATTLEMANAGER_HPP
#include <ALC\General.hpp>
#include <ALC\Bullets.hpp>

class Character;

enum class CharaType : ALC::uint8 {
	None, Alice, Rui, Eden
};

class BattleManager final {
	ALC_NON_CONSTRUCTABLE(BattleManager);
public:

	static ALC::rect GetDeathBounds() { return s_deathBounds; }
	static void SetDeathBounds(const ALC::rect& deathBounds) { s_deathBounds = deathBounds; }

	static CharaType GetCharacter() { return s_type; }
	static void SetCharcter(const CharaType type) { s_type = type; }

	// initializes this entity as whatever CharaType is stored in the battlemanager
	// returns null if entity is invalid or CharaType is None
	static Character* InitAsCharacter(ALC::Entity entity);

private:
	static ALC::rect s_deathBounds;
	static CharaType s_type;
};

#endif // !ALICE_BATTLEMANAGER_HPP