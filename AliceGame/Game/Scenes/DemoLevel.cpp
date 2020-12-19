#include "DemoLevel.hpp"

DemoLevel::DemoLevel() { }

DemoLevel::~DemoLevel() { }

void DemoLevel::Init() {
	// set the context
	ALC::ContentManager::SetContext(m_storage);

	// setup camera
	m_camera.SetCameraSize(m_camera.GetCameraSize() * 0.3f);
	ALC::vec2 camsize = m_camera.GetCameraSize();

	// create our player
	m_reg.Create().AddBehavior<DemoChara>();
}

void DemoLevel::Exit() { }

void DemoLevel::Step(ALC::Timestep t) {
	m_reg.UpdateBehaviors(t);

	ALC::vec2 bounds = m_camera.GetCameraSize() * 0.5f;
	const float delta = ALC::SceneManager::GetActiveGame()->GetTimer()->GetDelta();
	m_reg.ForeachComponent<ALC::Transform2D, ALC::Rigidbody2D>(
		[delta, bounds](ALC::Entity e, ALC::Transform2D& t, ALC::Rigidbody2D& r) {
		t.position += r.velocity * delta;

		//if (t.position.x < -bounds.x) {
		//	r.velocity.x = -r.velocity.x;
		//	t.position.x += 0.1f;
		//}
		//
		//if (t.position.x > bounds.x) {
		//	r.velocity.x = -r.velocity.x;
		//	t.position.x -= 0.1f;
		//}
		//
		//if (t.position.y < -bounds.y) {
		//	r.velocity.y = -r.velocity.y;
		//	t.position.y += 0.1f;
		//}
		//
		//if (t.position.y > bounds.y) {
		//	r.velocity.y = -r.velocity.y;
		//	t.position.y -= 0.1f;
		//}
	});

	// create all entities
	m_ech.CreateEntities(m_reg);

	m_reg.ForeachComponent<DemoBulletComponent>(
		[delta, this](ALC::Entity e, DemoBulletComponent& bul) {
		bul.lifetime += delta;
		if (bul.lifetime > bul.maxlifetime) {
			m_reg.DestroyEntity(e);
			//ALC_DEBUG_LOG("Destroy!");
		}
	});

	m_reg.LateUpdateBehaviors(t);
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

void DemoLevel::PostDraw() { }
