#include "BattleDemoLevel.hpp"

RTTR_REGISTRATION {
	registration::class_<BattleDemoLevel>("BattleDemoLevel")
		.public_object_constructor;
}

BattleDemoLevel::BattleDemoLevel() { }

BattleDemoLevel::~BattleDemoLevel() { }

void BattleDemoLevel::Init() {
	
	camera = ObjectFactory::Make<BattleDemoCamera>();

}

void BattleDemoLevel::Update() {

}

void BattleDemoLevel::LateUpdate() {

}

void BattleDemoLevel::Exit() {

}
