#ifndef PLAYER_BTPLAYER_H
#define PLAYER_BTPLAYER_H

#include "Spells/SpellList.hpp"
#include "BattleManager.hpp"

class BTSpell;
class BTPlayer : public Object {
	float moveSpeed;

	int hitpoints;
	int maxHitpoints;

	CircleCollider* collider;
	SpriteRenderer* sprite;

	array<BTSpell*, MAX_EQUIPPED_SPELLS> spells;
	uint32 curSpell;
public:
	BTPlayer();
	virtual ~BTPlayer() = 0;

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void UpdateHitpoints(int _modifier);

	// Getters and Setters
	float GetMoveSpeed();
	int GetHitpoints();
	int GetMaxHitpoints();

	SpriteRenderer* GetSprite();
	CircleCollider* GetCollider();

	void SetMoveSpeed(float _moveSpeed);
	void SetHitpoints(int _hitpoints);
	void SetMaxHitpoints(int _maxHitpoints, bool _updateCurHP);

	void SetSprite(const string& _texture, vec2 _pivot, int _layer);
	void SetCollider(float _radius);

	void SetBattleSkills(array<string, MAX_EQUIPPED_SPELLS> _spells);

	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

#endif

