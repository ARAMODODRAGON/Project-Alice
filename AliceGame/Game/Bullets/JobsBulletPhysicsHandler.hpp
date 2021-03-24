#ifndef ALICE_JOBSBULLETSPHYSICSHANDLER_HPP
#define ALICE_JOBSBULLETSPHYSICSHANDLER_HPP
#include "BulletPhysics.hpp"
#include <ALC/Entities/Registry.hpp>
#include <ALC/Jobs.hpp>

class JobsBulletsPhysicsHandler final {
	template<typename... T>
	using get_t = entt::get_t<T...>;
	template<typename... T>
	using exclude_t = entt::exclude_t<T...>;

	using char_group = entt::group<exclude_t<BulletBody>, get_t<ALC::Transform2D, ALC::EntityInfo>, CharacterBody>;
	using bult_group = entt::group<exclude_t<CharacterBody>, get_t<ALC::Transform2D, ALC::EntityInfo>, BulletBody>;
public:

	JobsBulletsPhysicsHandler(ALC::Registry& registry);
	~JobsBulletsPhysicsHandler();

	void Step(ALC::Timestep t);

private:

	// pairs for each charcter
	struct CharPair final {
		ALC::Entity entity;
		cjs::mutex lock;
		CharPair() = default;
		CharPair(const CharPair&) { /* DO NOTHING */ }
		CharPair(CharPair&&) { /* DO NOTHING */ }
	};

	// iterates on a chunk of bullets to update
	struct BulletJob final : ALC::IJob {
		ALC::Timestep ts;
		bult_group* bgroup = nullptr;
		ALC::vector<CharPair>* charpairs = nullptr;
		ALC::uint32 begin = 0;
		ALC::uint32 end = 0;
		void execute() override;
	};

	ALC::Registry& m_reg;
	ALC::vector<BulletJob> m_bultjobs;
	BulletJob m_mthrjob;
	ALC::vector<CharPair> m_charpairs;

};

#endif // !ALICE_JOBSBULLETSPHYSICSHANDLER_HPP