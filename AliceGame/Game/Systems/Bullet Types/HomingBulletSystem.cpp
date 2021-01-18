#include "HomingBulletSystem.hpp"

#define VEC2_UP ALC::vec2(0.0f, 1.0f)
#define VEC3_UP ALC::vec2(0.0f, 1.0f, 0.0f)

HomingBulletSystem::HomingBulletSystem(ALC::Registry& reg)
	: m_reg(reg) { }

HomingBulletSystem::~HomingBulletSystem() { }

void HomingBulletSystem::Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, ALC::Transform2D& tr, HomingBullet& homing) {
	// check if this entity needs to decide what target to choose
	if (homing.__targetID == -1) {
		// temp: target only the first target
		homing.__targetID = 0;
	}

	// move towards the target

	// get the target entity
	auto entity = m_reg.GetEntity(m_targets[homing.__targetID]);
	auto tarTr = entity.GetComponent<ALC::Transform2D>();

	// target vector
	ALC::vec2 targetvec = glm::normalize(tarTr.position - tr.position);
	float targetangle = ALC_TO_DEGREES(glm::orientedAngle(VEC2_UP, targetvec));

	// current angle
	ALC::vec2 curvec = glm::normalize(bb.velocity);
	float curangle = ALC_TO_DEGREES(glm::orientedAngle(VEC2_UP, curvec));

	// update speed
	homing.rotationSpeed += homing.rotationAccel * ts;

	// update angle
	if (ALC::NearlyEqual(curangle, targetangle, homing.rotationSpeed * ts))
		// move directly
		bb.velocity = glm::length(bb.velocity) * targetvec;
	else if (targetangle > curangle)
		// turn
		bb.velocity = ALC::vec2(glm::rotateZ(ALC::vec3(bb.velocity, 0.0f), ALC_TO_RADIANS(homing.rotationSpeed * ts)));
	else
		// turn
		bb.velocity = ALC::vec2(glm::rotateZ(ALC::vec3(bb.velocity, 0.0f), -ALC_TO_RADIANS(homing.rotationSpeed * ts)));

}

void HomingBulletSystem::AddTarget(ALC::EntityID target) {
	// confirm it doesnt already have the target
	for (size_t i = 0; i < m_targets.size(); i++) {
		if (m_targets[i] == target) return;
	}
	// add
	m_targets.push_back(target);
}

void HomingBulletSystem::RemoveTarget(ALC::EntityID target) {
	for (auto it = m_targets.begin(); it != m_targets.end(); ++it) {
		if (*it == target) {
			*it = nullptr;
			return;
		}
	}
	// error
	ALC_DEBUG_WARNING("target could not be removed, it was not found");
}
