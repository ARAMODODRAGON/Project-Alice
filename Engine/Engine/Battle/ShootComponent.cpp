#include "ShootComponent.hpp"

RTTR_REGISTRATION{
	registration::class_<ShootComponent>("ShootComponent")
		.public_object_constructor;
}

ShootComponent::ShootComponent() { }

ShootComponent::~ShootComponent() { }

void ShootComponent::Start() { }

void ShootComponent::OnDestroy() {
	for (auto set : bulletSet) { // Delete all allocated memory for the bullet sets
		for (auto bullet : set.bullets) {
			ObjectFactory::Destroy(bullet);
		}
		set.bullets.clear();
	}
	bulletSet.clear();	
}

void ShootComponent::Allocate(rttr::type _object, int _total) {
	for (auto set : bulletSet) { // Loop through the available sets of bullets 
		if (set.bulletType == _object) {
			DEBUG_LOG("Bullets for that type have already been allocated.");
			return; // Bullets couldn't be allocated; return false
		}
	}

	// Instantiate all the required bullets and deactivate them
	vector<BTBullet*> bullets(_total);
	for (int i = 0; i < _total; i++) {
		bullets[i] = new BTBullet();
		bullets[i]->SetIsActive(false);
	}
	DEBUG_LOG(VTOS(_total) + " bullets of type BTBullet have been allocated.");

	// Finally, add the set of bullets to the main bullet set and return true
	BulletSet newSet(bullets, _object);
	bulletSet.push_back(newSet);
}

void ShootComponent::Shoot(rttr::type _object, int _total, std::function<void(BTBullet*)> _func) {
	if (_func == nullptr) { // If no function was provided; throw an error
		DEBUG_ERROR("No function exists! Cannot create bullet!");
		return;
	}

	// Loop through all sets and find the bullets that match the type needed for creation
	for (auto set : bulletSet) {
		if (set.bulletType == _object) {
			for (int i = 0; i < set.bullets.size(); i++) {
				if (!set.bullets[i]->GetIsActive()) {
					_func(set.bullets[i]);
					_total--;
					DEBUG_LOG("Fired");
				}
				// Break out of the loop early if all required bullets have been created
				if (_total <= 0) {
					//DEBUG_WARNING("Not enough bullets have been allocated. Number needed to shoot remaining is " + VTOS(_total) + " bullets.");
					break;
				}
			}
		}
	}
}		  