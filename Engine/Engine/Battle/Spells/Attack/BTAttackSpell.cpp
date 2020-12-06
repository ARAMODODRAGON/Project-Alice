#include "BTAttackSpell.hpp"

BTAttackSpell::BTAttackSpell()
	: allocateCount(0)
	, shootComponent(nullptr)
{}

BTAttackSpell::~BTAttackSpell() {
	shootComponent->OnDestroy();
}

ShootComponent* BTAttackSpell::GetShootComponent() {
	return shootComponent;
}

void BTAttackSpell::SetShootComponent(ShootComponent* _component) {
	if (shootComponent == nullptr) {
		shootComponent = _component;
	}
}