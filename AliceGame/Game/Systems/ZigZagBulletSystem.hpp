#ifndef ZIG_ZAG_BULLET_SYSTEM_HPP
#define ZIG_ZAG_BULLET_SYSTEM_HPP
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>
#include <ALC\Physics\Transform2D.hpp>

struct ZigZagBullets {
	float timeToShift = 0.0f;
    float lifeSpan = 2.5f;

    bool posShift = false;

};

struct ZigZagBulletSystem final :public ALC::ISystem<ALC::BulletBody, ZigZagBullets>
{
    ZigZagBulletSystem();
    ~ZigZagBulletSystem();
    
    void Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, ZigZagBullets& zzBul) override;
};
#endif

