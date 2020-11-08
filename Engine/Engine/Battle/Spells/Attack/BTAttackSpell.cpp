#include "BTAttackSpell.hpp"

RTTR_REGISTRATION{
	registration::class_<BTAttackSpell>("BTAttackSpell")
		.property("type", &BTAttackSpell::attackType)
		.property("count", &BTAttackSpell::allocateCount);
}

BTAttackSpell::BTAttackSpell(rttr::type _type, int _count) 
	: attackType(_type)
	, allocateCount(_count)
	, shootComponent(nullptr)
{}

ShootComponent* BTAttackSpell::GetShootComponent() {
	return shootComponent;
}

void BTAttackSpell::SetShootComponent(ShootComponent* _component) {
	if (shootComponent == nullptr) {
		shootComponent = _component;
		shootComponent->Allocate(attackType, allocateCount);
	}
}