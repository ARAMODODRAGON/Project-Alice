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

	// load font
	m_arielFont = ALC::Font::Load("Resources/Fonts/arial.ttf", 50);

	// create our player
	m_reg.Create().AddBehavior<DemoChara>();

	m_reg.Create();
	auto e = m_reg.Create();
	m_eid = e.GetComponent<ALC::EntityInfo>().GetID();
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

void DemoLevel::Exit() {
	// delete font
	ALC::Font::Delete(m_arielFont);
	m_arielFont = nullptr;
}

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

	ALC::Entity e = m_reg.GetEntity(m_eid);
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

	// draw sprites
	m_batch.Begin(m_camera);

	m_reg.ForeachComponent<ALC::Transform2D, ALC::SpriteComponent>(
		[this](ALC::Entity e, ALC::Transform2D& transform, ALC::SpriteComponent& sprite) {
		m_batch.Draw(transform, sprite);
	});

	m_batch.End();

	// draw UI
	m_ui.Begin();
	const ALC::vec2 screensize = ALC::SceneManager::GetWindow()->GetScreenSize();
	const ALC::vec2 halfscreensize = screensize * 0.5f;

	ALC::rect levelBounds;
	float toppos = halfscreensize.y - 15.0f;
	constexpr float ratio = 0.88f;
	levelBounds.top = toppos + halfscreensize.y;
	levelBounds.bottom = -toppos + halfscreensize.y;
	levelBounds.right = toppos * ratio + halfscreensize.x;
	levelBounds.left = -toppos * ratio + halfscreensize.x;
	//m_ui.DrawQuad(levelBounds, ALC_COLOR_GREEN);

	// create screen to world matrix
	ALC::mat4 screenToWorld = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y);
	screenToWorld = screenToWorld * glm::inverse(m_camera.GetTransform());

	// convert level bounds from screen to world
	ALC::vec4 deathmin, deathmax;
	deathmin = ALC::vec4(levelBounds.min - halfscreensize, 0.0f, 1.0f) * screenToWorld;
	deathmax = ALC::vec4(levelBounds.max - halfscreensize, 0.0f, 1.0f) * screenToWorld;
	// set 
	m_bdeleter.SetDeathBoundry(ALC::rect(deathmin.x, deathmin.y, deathmax.x, deathmax.y));

	// draw the level boundries
	ALC::rect r;
	constexpr float borderWidth = 10.0f;
	{
		// left
		r.bottom = levelBounds.bottom - borderWidth;
		r.top = levelBounds.top + borderWidth;
		r.right = levelBounds.left;
		r.left = r.right - borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_BLUE);

		// right
		r.bottom = levelBounds.bottom - borderWidth;
		r.top = levelBounds.top + borderWidth;
		r.left = levelBounds.right;
		r.right = r.left + borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_BLUE);

		// top
		r.left = levelBounds.left;
		r.right = levelBounds.right;
		r.bottom = levelBounds.top;
		r.top = r.bottom + borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_BLUE);

		// bottom
		r.left = levelBounds.left;
		r.right = levelBounds.right;
		r.top = levelBounds.bottom;
		r.bottom = r.top - borderWidth;
		m_ui.DrawQuad(r, ALC_COLOR_BLUE);
	}

	m_ui.DrawText("Hello there General Kenobi", m_arielFont, ALC::vec2(100.0f, 100.0f));

	m_ui.End();
}

void DemoLevel::PostDraw() {
	m_reg.Cleanup();
	m_ech.Cleanup(m_reg);
}
