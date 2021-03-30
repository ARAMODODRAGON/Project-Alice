#ifndef ALICE_BULLETPHYSICSSYSTEM_HPP
#define ALICE_BULLETPHYSICSSYSTEM_HPP
#include "BulletPhysics.hpp"
#include <ALC/Entities.hpp>
#include <ALC/Jobs.hpp>

struct BulletPhysicsSystem final : public ALC::ISystem<BulletBody, ALC::Transform2D, ALC::EntityInfo> {

	BulletPhysicsSystem(ALC::Registry& reg);
	~BulletPhysicsSystem();

	void UpdateCharacters(ALC::Timestep ts);
	virtual void Step(ALC::Timestep ts, ALC::Entity e, BulletBody& bb, ALC::Transform2D& tr, ALC::EntityInfo& ei);

private:

	// pairs for each charcter
	struct CharPair final {
		ALC::Entity entity;
		std::mutex lock;
		CharPair() = default;
		CharPair(const CharPair&) { /* DO NOTHING */ }
		CharPair(CharPair&&) { /* DO NOTHING */ }
	};

	ALC::Registry& m_reg;
	ALC::vector<CharPair> m_charpairs;

};

#endif // !ALICE_BULLETPHYSICSSYSTEM_HPP