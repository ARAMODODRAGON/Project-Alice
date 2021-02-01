#include "Battle2.hpp"

Battle2::Battle2()
	: m_enemy(nullptr)
	, m_deletersys(GetECH())
	, m_homingsys(GetReg()) { }

Battle2::~Battle2() { }

void Battle2::Init() {

	// default player
	if (BattleManager::GetCharacter() == CharaType::None)
		BattleManager::SetCharacter(CharaType::Rui);

	BattleLevel::Init();

	auto eenemy = GetReg().Create();
	m_enemy = eenemy.AddBehavior<EdenEnemy>();
	m_homingsys.AddTarget(eenemy.GetComponent<ALC::EntityInfo>().GetID());

}

void Battle2::Exit() {
	BattleLevel::Exit();
}

void Battle2::GameStep(ALC::Timestep ts) { 

	GetReg().StepSystem(ts, m_homingsys);
	GetReg().StepSystem(ts, m_deletersys);

}

void Battle2::Draw() {
	BattleLevel::Draw();
}
