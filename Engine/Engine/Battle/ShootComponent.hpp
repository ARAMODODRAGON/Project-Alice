#ifndef COMPONENT_SHOOT_HPP
#define COMPONENT_SHOOT_HPP

#include "../Common.hpp"

// TEMPORARY CLASS
class BTBullet : public Object {
	SpriteRenderer* sprite;
public:
	void SetSprite(std::string spritePath_, glm::vec2 scale_) {
		sprite = AddComponent<SpriteRenderer>();
		sprite->LoadTexture(spritePath_);
		sprite->SetScale(scale_);
		sprite->SetTilingSize(glm::vec2(16.0f));
	}
};

class ShootComponent : public Component {

	struct BulletSet {
		vector<BTBullet*> bullets;
		uint32 typeID;

		BulletSet(vector<BTBullet*> _bullets, uint32 _typeID) {
			bullets = _bullets;
			typeID = _typeID;
		}
	};
	list<BulletSet> bulletSet;

public:
	ShootComponent();
	~ShootComponent();

	void Start() override;
	void OnDestroy() override;

	template<class T>
	void Allocate(int _total);
	template<class T, typename Func>
	void Shoot(int _total, Func _func);

};

template<class T>
inline void ShootComponent::Allocate(int _total) {
	const uint32 tyID = GetTypeID<T>();
	for (auto set : bulletSet) { // Loop through the available sets of bullets 
		if (set.typeID != tyID) continue;

		DEBUG_LOG("Bullets for that type have already been allocated.");
		return; // Bullets couldn't be allocated; return false
	}

	// Instantiate all the required bullets and deactivate them
	vector<BTBullet*> bullets(_total);
	for (int i = 0; i < _total; i++) {
		bullets[i] = new T();
		bullets[i]->SetIsActive(false);
	}
	DEBUG_LOG(VTOS(_total) + " bullets of type BTBullet have been allocated.");

	// Finally, add the set of bullets to the main bullet set and return true
	BulletSet newSet(std::move(bullets), tyID);
	bulletSet.push_back(newSet);
}

template<class T, typename Func>
inline void ShootComponent::Shoot(int _total, Func _func) {
	const uint32 tyID = GetTypeID<T>();
	// Loop through all sets and find the bullets that match the type needed for creation
	for (auto set : bulletSet) {
		if (set.typeID == tyID) {
			for (int i = 0; i < set.bullets.size(); i++) {
				if (!set.bullets[i]->GetIsActive()) {
					set.bullets[i]->SetIsActive(true);
					_func(set.bullets[i]);
					_total--;
					//DEBUG_LOG("Fired");
				}
				// Break out of the loop early if all required bullets have been created
				if (_total <= 0) break;
				if (i + 1 == set.bullets.size()) {
					DEBUG_WARNING("Not enough bullets have been allocated. Number needed to shoot remaining is " + VTOS(_total) + " bullets.");
					break;
				}
			}
			break;
		}
	}
}

#endif