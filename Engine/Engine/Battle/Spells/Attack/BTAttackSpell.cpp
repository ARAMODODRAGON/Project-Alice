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

BTAttackSpell::~BTAttackSpell() {
	shootComponent->OnDestroy();
	//delete shootComponent, shootComponent = nullptr;
}

void BTAttackSpell::Update() {
	// TODO -- Add shooty shooty bang bang here
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