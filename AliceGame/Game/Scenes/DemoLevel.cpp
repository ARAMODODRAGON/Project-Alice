#include "DemoLevel.hpp"

DemoLevel::DemoLevel() : m_bdeleter(m_ech) { }

DemoLevel::~DemoLevel() { }

void DemoLevel::Init() {
	// set the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera
	m_camera.SetCameraSize(m_camera.GetCameraSize() * 0.3f);
	ALC::vec2 halfcamsize = m_camera.GetCameraSize() * 0.5f;
	m_bdeleter.SetDeathBoundry(ALC::rect(-halfcamsize, halfcamsize));

	// create our player
	m_reg.Create().AddBehavior<DemoChara>();

	m_reg.Create();
	auto e = m_reg.Create();
	eid = e.GetComponent<ALC::EntityInfo>().GetID();
	e.AddComponent<ALC::Transform2D>();
	auto& rb = e.AddComponent<ALC::CharacterBody>();
	rb.mask = ALC::Layermask32::ALL;
	rb.radius = 8.0f;
	auto& spr = e.AddComponent<ALC::SpriteComponent>();
	spr.bounds = ALC::rect(-8.0f, -8.0f, 8.0f, 8.0f);
	spr.color = ALC_COLOR_GREEN;
	spr.texture = ALC::ContentManager::LoadTexture("Resources/Textures/circle.png");
	spr.textureBounds = spr.texture.GetBounds();
}

void DemoLevel::Exit() { }

void DemoLevel::Step(ALC::Timestep t) {
	// force a timestep of 1 / 60
	static ALC::Timestep ts = ALC::Timestep(1.0 / 60.0);

	m_reg.UpdateBehaviors(ts);

	//ALC::vec2 bounds = m_camera.GetCameraSize() * 0.5f;
	//const float delta = ALC::SceneManager::GetActiveGame()->GetTimer()->GetDelta();
	//m_reg.ForeachComponent<ALC::Transform2D, ALC::Rigidbody2D>(
	//	[delta, bounds](ALC::Entity e, ALC::Transform2D& t, ALC::Rigidbody2D& r) {
	//	t.position += r.velocity * delta;
	//
	//	//if (t.position.x < -bounds.x) {
	//	//	r.velocity.x = -r.velocity.x;
	//	//	t.position.x += 0.1f;
	//	//}
	//	//
	//	//if (t.position.x > bounds.x) {
	//	//	r.velocity.x = -r.velocity.x;
	//	//	t.position.x -= 0.1f;
	//	//}
	//	//
	//	//if (t.position.y < -bounds.y) {
	//	//	r.velocity.y = -r.velocity.y;
	//	//	t.position.y += 0.1f;
	//	//}
	//	//
	//	//if (t.position.y > bounds.y) {
	//	//	r.velocity.y = -r.velocity.y;
	//	//	t.position.y -= 0.1f;
	//	//}
	//});

	m_bPhysics.Step(m_reg, ts);

	// create all entities
	m_ech.Cleanup(m_reg);

	ALC::Entity e = m_reg.GetEntity(eid);
	auto [rb, spr] = e.GetComponent<ALC::CharacterBody, ALC::SpriteComponent>();
	if (rb.Count() > 0)
		spr.color = ALC_COLOR_WHITE;
	else
		spr.color = ALC_COLOR_GREEN;

	m_reg.LateUpdateBehaviors(ts);

	m_reg.StepSystem(ts, m_bdeleter);
}

void DemoLevel::PreDraw() { }

void DemoLevel::Draw() {
	m_batch.Begin(m_camera);

	m_reg.ForeachComponent<ALC::Transform2D, ALC::SpriteComponent>(
		[this](ALC::Entity e, ALC::Transform2D& transform, ALC::SpriteComponent& sprite) {
		m_batch.Draw(transform, sprite);
	});

	m_batch.End();
}

void DemoLevel::PostDraw() {
	m_reg.Cleanup();
	m_ech.Cleanup(m_reg);
}
