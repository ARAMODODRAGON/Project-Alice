#include "JobsBulletPhysicsHandler.hpp"

using mutex_guard = std::lock_guard<std::mutex>;
#define CHARACTER_GROUP_P get_t<ALC::Transform2D, ALC::EntityInfo>{}, exclude_t<BulletBody>{}
#define BULLET_GROUP_P get_t<ALC::Transform2D, ALC::EntityInfo>{}, exclude_t<CharacterBody>{}

JobsBulletsPhysicsHandler::JobsBulletsPhysicsHandler(ALC::Registry& registry)
	: m_reg(registry) {
	auto& r = m_reg.__GetReg();

	// prepare the component pools
	r.prepare<ALC::Transform2D>();
	r.prepare<CharacterBody>();
	r.prepare<BulletBody>();

	// prepare the groups
	char_group charg = r.group<CharacterBody>(CHARACTER_GROUP_P);
	bult_group bultg = r.group<BulletBody>(BULLET_GROUP_P);

	// prepare the bultjobs
	m_bultjobs.resize(ALC::JobQueue::GetWorkerCount());
	for (auto& j : m_bultjobs) j.charpairs = &m_charpairs;
	m_mthrjob.charpairs = &m_charpairs;

}

JobsBulletsPhysicsHandler::~JobsBulletsPhysicsHandler() { }

void JobsBulletsPhysicsHandler::Step(ALC::Timestep ts) {
	auto& r = m_reg.__GetReg();

	// get groups
	char_group charg = r.group<CharacterBody>(CHARACTER_GROUP_P);
	bult_group bultg = r.group<BulletBody>(BULLET_GROUP_P);

	// make sure there are enough charpairs
	if (charg.size() != m_charpairs.size()) m_charpairs.resize(charg.size());

	// update characters single threaded
	for (size_t i = 0; i < charg.size(); i++) {
		m_charpairs[i].entity = ALC::Entity(charg[i], &m_reg);
		auto [tr, cb] = charg.get<ALC::Transform2D, CharacterBody>(charg[i]);

		// motion
		tr.position += cb.velocity * ts;

		// set the list of collisions 
		auto& collisioninfo = cb.__GetColl();
		for (auto it = collisioninfo.begin(); it != collisioninfo.end();) {
			if (it->GetState() == ALC::CollisionState::End) {
				it = collisioninfo.erase(it);
				continue;
			}
			it->__SetState(ALC::CollisionState::End);
			++it;
		}
	}

	// update bullets multithreaded
	const size_t chunksize = bultg.size() / m_bultjobs.size();
	const size_t extrasize = bultg.size() % m_bultjobs.size();
	
	ALC::Handle handle;

	size_t pos = 0;
	if (chunksize > 1) {
		for (size_t i = 0; i < m_bultjobs.size(); i++) {
			auto& j = m_bultjobs[i];

			// decide the portion
			size_t chunk = chunksize;
			if (i == 0) chunk += extrasize;

			// set portion
			j.begin = pos;
			pos += chunk;
			j.end = pos;

			// set group, timestep and submit
			j.bgroup = &bultg;
			j.ts = ts;
			handle += &j;
		}
	}

	ALC::JobQueue::Submit(&handle);

	// last few remaining bullets done in extra job
	m_mthrjob.bgroup = &bultg;
	m_mthrjob.ts = ts;
	m_mthrjob.begin = pos;
	m_mthrjob.end = bultg.size();
	if (m_mthrjob.begin < m_mthrjob.end) m_mthrjob.execute();

	// await
	handle.await_complete();

}

void JobsBulletsPhysicsHandler::BulletJob::execute() {
	if (!charpairs || !bgroup || (end <= begin)) {
		ALC_DEBUG_ERROR("BulletJob not setup correctly");
		return;
	}

	for (size_t i = begin; i < end; i++) {
		auto [tr, bb] = bgroup->get<ALC::Transform2D, BulletBody>(bgroup->operator[](i));
		if (!bb.isSimulated || bb.radius < 0.0001f) continue;

		tr.position += bb.velocity * ts;
		bb.__SetCol(nullptr);

		// test for collisions
		for (size_t i = 0; i < charpairs->size(); i++) {
			auto& chp = charpairs->operator[](i);

			auto [ct, cb] = chp.entity.GetComponent<ALC::Transform2D, CharacterBody>();
			if (!cb.isSimulated || cb.radius < 0.0001f) continue;

			// check layermask
			if (!(cb.mask && bb.mask)) continue;

			// determine overlap
			if (float mindist = bb.radius + cb.radius; 
				glm::length2(tr.position - ct.position) >= (mindist * mindist)) continue;

			// lock and write collision info
			mutex_guard _(chp.lock);

			// get some stuff
			auto& bi = bgroup->get<ALC::EntityInfo>(bgroup->operator[](i));
			auto& ci = chp.entity.GetComponent<ALC::EntityInfo>();
			auto& collinfo = cb.__GetColl();

			// update entry
			for (size_t i = 0; i < collinfo.size(); i++) {
				if (collinfo[i].GetOther() == bi.GetID()) {
					collinfo[i].__SetState(ALC::CollisionState::Stay);
					break;
				}
			}

			// new entry
			BCollisionInfo cinfo(0, bb.mask & cb.mask, bi.GetID(), ALC::CollisionState::Begin, bb.damage);
			collinfo.push_back(cinfo);

			// mark the bullet so it knows it collided with this character
			bb.__SetCol(ci.GetID());

			break;
		}
	}

	bgroup = nullptr;
	end = begin = 0;
}