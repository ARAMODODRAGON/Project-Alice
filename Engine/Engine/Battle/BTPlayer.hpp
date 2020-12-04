#ifndef PLAYER_BTPLAYER_H
#define PLAYER_BTPLAYER_H

#include "Spells/SpellList.hpp"
#include "BattleManager.hpp"

class BTAttackSpell;
class BTDefenceSpell;
class BTPlayer : public Object {
public:
	using ATKSpells = array<BTAttackSpell*, MAX_EQUIPPED_SPELLS>;

	BTPlayer();
	virtual ~BTPlayer() = 0;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void UpdateHitpoints(int _modifier);

	// Getters and Setters
	float GetMoveSpeed() { return moveSpeed; }
	float GetSlowSpeed() { return slowSpeed; }
	int GetHitpoints() { return hitpoints; }
	int GetMaxHitpoints() { return maxHitpoints; }

	SpriteRenderer* GetSprite() { return sprite; }
	CircleCollider* GetCollider() { return collider; }

	void SetMoveSpeed(float _moveSpeed) { moveSpeed = _moveSpeed; }
	void SetSlowSpeed(float _slowSpeed) { slowSpeed = _slowSpeed; }
	void SetHitpoints(int _hitpoints) { UpdateHitpoints(_hitpoints - hitpoints); }
	void SetMaxHitpoints(int _maxHitpoints, bool _updateCurHP);

	void SetSprite(const string& _texture, vec2 _pivot, int _layer);
	void SetCollider(float _radius);

	void SetBattleSpells(array<string, MAX_EQUIPPED_SPELLS> _atkSpells, const string& _defSpell);
	// sets the spells directly
	void SetBattleSpells(ATKSpells spells, BTDefenceSpell* spell);
private:

	float moveSpeed;
	float slowSpeed;

	int hitpoints;
	int maxHitpoints;

	CircleCollider* collider;
	SpriteRenderer* sprite;

	BTDefenceSpell* defenceSpell;
	ATKSpells atkSpells;
	uint8 curAtkSpell;

	void DeleteSpells();

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif 