#ifndef ALICE_CHARACTERS_CHARACTER_HPP
#define ALICE_CHARACTERS_CHARACTER_HPP
#include <ALC\Bullets.hpp>
#include <ALC\Input.hpp>
#include <ALC\Rendering.hpp>
#include "../BattleManager.hpp"

class Character : public ALC::ShooterBehavior {
public:

	Character();
	virtual ~Character() = 0;

	ALC::vec2 GetInputAxis() const;
	ALC::Button GetShootButton();
	ALC::Button GetSlowButton();

	float GetMaxSpeed() const { return m_maxSpeed; }
	void SetMaxSpeed(const float maxSpeed) { m_maxSpeed = maxSpeed; }
	float GetSlowScalar() const { return m_slowScalar; }
	void SetSlowScalar(const float slowScalar) { m_slowScalar = slowScalar; }

	// call this to update the characters movement
	// and clamp them within the BattleManager::GetLevelBounds()
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts);

	// alternate call with more inputs
	void UpdateMovement(ALC::Entity self, ALC::Timestep ts, const ALC::vec2& inputAxis, const float speed);

	// garuntees that this entity has a character body, transform, and spritecomponent
	virtual void Start(ALC::Entity self);

private:
	float m_maxSpeed;
	float m_slowScalar;
};

#endif // !ALICE_CHARACTERS_CHARACTER_HPP