#include "TestEnemy.hpp"

RTTR_REGISTRATION{
	registration::class_<TestEnemy>("TestEnemy")
		.public_object_constructor
		.property("destination",&TestEnemy::destination);
}


TestEnemy::TestEnemy() : sprite(nullptr), destination(vec2()), isMoving(false) {}

TestEnemy::~TestEnemy()
{
	OnDestroy();
}

void TestEnemy::SeekAndArrive(bool isMoving_)
{
	vec2 destination_;

	if (!isMoving_) {
		destination_ = vec2(rand() % 8 | rand() % 8 * -1, rand() % 8 | rand() % 8 * -1);
		vec2 tempVel;
		tempVel = destination_ - this->GetPosition();
		normalize(tempVel);
		tempVel *= movmentSpeed;
		this->SetVelocity(tempVel);
		isMoving = true;
		SetDestination(destination_);
	}
	
	if (isMoving_ && roundEven(GetPosition()) == GetDestination()) {
		isMoving = false;
	}
		
	
	cout << "Enemy Pos : " << "(" << GetPosition().x << "," << GetPosition().y << endl;
	cout << "Destination : " << "(" << GetDestination().x <<  "," << GetDestination().y << endl;
}

void TestEnemy::Start()
{
	sprite = AddComponent<Sprite>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
	srand(time(NULL));
	
}

void TestEnemy::Update()
{
	 SeekAndArrive(isMoving); 
}

void TestEnemy::LateUpdate()
{
	
	cout << "I call functions " << endl;
}

void TestEnemy::OnDestroy()
{
}