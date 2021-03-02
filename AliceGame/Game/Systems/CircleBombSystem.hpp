#ifndef CIRCLE_BOMB_SYSTEM_HPP
#define CIRCLE_BOMB_SYSTEM_HPP
#include <ALC\Entities\Registry.hpp>
#include <ALC\Entities\EntityCreationHandler.hpp>
#include <ALC\Bullets\BulletPhysics.hpp>
#include <ALC\Rendering\SpriteBatch.hpp>

struct CircleBomb {
    // some data here (you need something to make this struct a valid component, even if you dont use the value)
    float timeToStop = 0.0f;
    float timeToExld = .3f; // time untill the bullets move
    bool canSetVel = true; // used to set the initial velocity for the bullet;
    ALC::vec2 initVel; // the intial velocity of the bullet
    float timeToStopFollow = 1.0f;
};

struct CircleBombSystem final : public ALC::ISystem<ALC::BulletBody, CircleBomb> {

    CircleBombSystem();
    ~CircleBombSystem();

    void Step(ALC::Timestep ts, ALC::Entity e, ALC::BulletBody& bb, CircleBomb& cirBmb) override;
    

private:
    
   
};

#endif
