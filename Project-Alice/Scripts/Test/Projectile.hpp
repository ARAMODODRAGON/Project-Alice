#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include <Engine/Common.hpp>

class Projectile : public Object {

	unsigned int timer;
	SpriteRenderer* sprite;
	vec4 color;
	unsigned int destroyTime;

public:

	Projectile();

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void SetColor(const vec4& color_) { color = color_; }
	SpriteRenderer* GetSprite() const { return sprite; }
	void SetDestroyTime(unsigned int destroyTime_) { destroyTime = destroyTime_; }

	RTTR_ENABLE(Object)
};

#endif // !PROJECTILE_HPP