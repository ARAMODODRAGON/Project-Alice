#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP
#include <Engine/Common.hpp>

class Projectile : public Object {

	uint32 timer;
	SpriteRenderer* sprite;
	vec4 color;
	uint32 destroyTime;

public:

	Projectile();

	void Start() override;
	void Update() override;
	void LateUpdate() override;

	void SetColor(const vec4& color_) { color = color_; }
	SpriteRenderer* GetSprite() const { return sprite; }
	void SetDestroyTime(uint32 destroyTime_) { destroyTime = destroyTime_; }

};

#endif // !PROJECTILE_HPP