#ifndef GRAVITY_BULLET_SYSTEM_HPP
#define GRAVITY_BULLET_SYSTEM_HPP
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>

struct GravityBullet {
    // some data here (you need something to make this struct a valid component, even if you dont use the value)
    float timeToFall = 0.0f;
    bool playerTracer;
};

struct GravityBulletSystem final : public ALC::ISystem<ALC::BulletBody,GravityBullet> {

    GravityBulletSystem();
    ~GravityBulletSystem();
  
    void Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, GravityBullet& grav) override;
    // set gravity function too lazy to type
    void SetGravity(const ALC::vec2 gravity_) { gravity = gravity_; }

private:
 
    ALC::vec2 gravity;
    //ALC::EntityCreationHandler& m_ech;
};

#endif
