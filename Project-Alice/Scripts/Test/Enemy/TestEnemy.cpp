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
	int randomNumberX = rand() % 2;
	int randomNumberY = rand() % 2;
	float tmpX, tmpY;
	int screenX = 95;
	int screenY = 50;

	//Generate a random x and y coord within the window
	if (randomNumberX == 1) {
		tmpX = rand() % screenX;
	}
	else { tmpX = rand() % screenX * -1; }

	if (randomNumberY == 1) {
		tmpY = rand() % screenY;
	}
	else { tmpY = rand() % screenY * -1; }


	if (!isMoving_) {
		destination_ = vec2(tmpX, tmpY);
		vec2 tempVel;
		tempVel = destination_ - GetPosition();
		//tempVel = normalize(tempVel);
		//tempVel *= GetMovementSpeed();
		SetVelocity((normalize(tempVel)) * GetMovementSpeed());
		isMoving = true;
		SetDestination(destination_);
	}

	if (isMoving_ ) {

		vec2 tmpPosition = ceil(GetPosition());

		//Enemy arrived at the destination 
		if (tmpPosition == GetDestination()) {
			DEBUG_LOG("Enemy Position is equal with Destination");
			isMoving = false;
			SetVelocity(vec2());
		}
		
		// if Enemy Passes the destination stop moving 
		else if (tmpPosition.x >= GetDestination().x && tmpPosition.y >= GetDestination().y) {
			DEBUG_LOG("Enemy Position is Greater than the destination ");
			isMoving = false;
			SetVelocity(vec2());

		}
	}
}

void TestEnemy::Start()
{
	sprite = AddComponent<Sprite>();
	sprite->LoadTexture("Flashing Grey Orb");
	sprite->SetTilingSize(vec2(16.0f));
	sprite->SetPivot(vec2(8.0f));
	sprite->SetLayer(2);
	srand(time(NULL));
	SetMovementSpeed(100.0f);
	
}

void TestEnemy::Update()
{
	  
}

void TestEnemy::LateUpdate()
{
	SeekAndArrive(isMoving);
}

void TestEnemy::OnDestroy()
{
}