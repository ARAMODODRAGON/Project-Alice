#ifndef ALICE_SCENES_DEMOLEVEL_HPP
#define ALICE_SCENES_DEMOLEVEL_HPP
#include <ALC\SceneManager.hpp>
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Bullets\BulletPhysicsHandler.hpp>
#include <ALC\Bullets\BaseBulletSystem.hpp>
#include "../Characters/DemoChara.hpp"

class DemoLevel final : public ALC::IScene {
public:

	DemoLevel();
	~DemoLevel();

	void Init() override;
	void Exit() override;
	void Step(ALC::Timestep t) override;
	void PreDraw() override;
	void Draw() override;
	void PostDraw() override;

private:

	ALC::ContentStorage m_storage;
	ALC::Registry m_reg;
	ALC::EntityCreationHandler m_ech;
	ALC::SpriteBatch m_batch;
	ALC::Camera m_camera;
	ALC::BulletPhysicsHandler m_bPhysics;
	ALC::EntityID eid;

	// systems
	ALC::BaseBulletSystem m_baseBulletSys;

};

#endif // !ALICE_SCENES_DEMOLEVEL_HPP