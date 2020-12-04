#ifndef COMPONENT_SHOOT_HPP
#define COMPONENT_SHOOT_HPP

#include "../Common.hpp"

// TEMPORARY CLASS
class BTBullet : public Object {
	SpriteRenderer* sprite;
public:
	void SetSprite(std::string spritePath_,glm::vec2 scale_) {
		sprite = AddComponent<SpriteRenderer>();
		sprite->LoadTexture(spritePath_);
		sprite->SetScale(scale_);
		sprite->SetTilingSize(glm::vec2(16.0f));
	}
	RTTR_ENABLE(Object) RTTR_REGISTRATION_FRIEND
};

class ShootComponent : public Component {

	struct BulletSet {
		vector<BTBullet*> bullets;
		rttr::type bulletType;

		BulletSet(vector<BTBullet*> _bullets, rttr::type _bulletType) : bulletType(type::get<void>()) {
			bullets = _bullets;
			bulletType = _bulletType;
		}
	};
	list<BulletSet> bulletSet;

public:
	ShootComponent();
	~ShootComponent();

	void Start() override;
	void OnDestroy() override;

	void Allocate(rttr::type _object, int _total);
	void Shoot(rttr::type _object, int _total, std::function<void(BTBullet*)> _func);

	RTTR_ENABLE(Component) RTTR_REGISTRATION_FRIEND
};

#endif