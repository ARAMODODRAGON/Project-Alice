#include "BTAttackSpell.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAttackSpell>("BTAttackSpell")
		.property("type", &BTAttackSpell::attackType)
		.property("count", &BTAttackSpell::allocateCount);
}

BTAttackSpell::BTAttackSpell()
	: attackType(type::get<void>())
	, allocateCount(0)
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
		shootComponent->Allocate(attackType, allocateCount);
	}
}