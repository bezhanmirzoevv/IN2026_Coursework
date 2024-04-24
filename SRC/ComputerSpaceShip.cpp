#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "ComputerSpaceShip.h"
#include "BoundingSphere.h"
#include "GameObject.h"
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <thread>  // for std::this_thread::sleep_for
#include <chrono>  // for std::chrono::milliseconds
#define _USE_MATH_DEFINES
#include <cmath>
#define RAD2DEG (180.0 / M_PI)

using namespace std;

// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/**  Default constructor. */
ComputerSpaceShip::ComputerSpaceShip()
	: GameObject("ComputerSpaceShip"), mThrust(0)
{
}

/** Construct a spaceship with given position, velocity, acceleration, angle, and rotation. */
ComputerSpaceShip::ComputerSpaceShip(GLVector3f p, GLVector3f v, GLVector3f a, GLfloat h, GLfloat r)
	: GameObject("ComputerSpaceShip", p, v, a, h, r), mThrust(0)
{
}

/** Copy constructor. */
ComputerSpaceShip::ComputerSpaceShip(const Spaceship& s)
	: GameObject(s), mThrust(0)
{
}

/** Destructor. */
ComputerSpaceShip::~ComputerSpaceShip(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Update this spaceship. */
void ComputerSpaceShip::Update(int t)
{
	// Call parent update function
	GameObject::Update(t);
	
	Navigation(mWorld->getGameObjects(), 2.0f);
}

/** Render this spaceship. */
void ComputerSpaceShip::Render(void)
{
	if (mSpaceshipShape.get() != NULL) mSpaceshipShape->Render();

	// If ship is thrusting
	if ((mThrust > 0) && (mThrusterShape.get() != NULL)) {
		mThrusterShape->Render();
	}

	GameObject::Render();
}

/** Fire the rockets. */
void ComputerSpaceShip::Thrust(float t)
{
	mThrust = t;
	// Increase acceleration in the direction of ship
	mAcceleration.x = mThrust * cos(DEG2RAD * mAngle);
	mAcceleration.y = mThrust * sin(DEG2RAD * mAngle);
}

/** Set the rotation. */
void ComputerSpaceShip::Rotate(float r)
{
	mRotation = r;
}

/** Shoot a bullet. */
void ComputerSpaceShip::Shoot(void)
{
	// Check the world exists
	if (!mWorld) return;
	// Construct a unit length vector in the direction the spaceship is headed
	GLVector3f spaceship_heading(cos(DEG2RAD * mAngle), sin(DEG2RAD * mAngle), 0);
	spaceship_heading.normalize();
	// Calculate the point at the node of the spaceship from position and heading
	GLVector3f bullet_position = mPosition + (spaceship_heading * 4);
	// Calculate how fast the bullet should travel
	float bullet_speed = 30;
	// Construct a vector for the bullet's velocity
	GLVector3f bullet_velocity = mVelocity + spaceship_heading * bullet_speed;
	// Construct a new bullet
	shared_ptr<GameObject> bullet
	(new Bullet(bullet_position, bullet_velocity, mAcceleration, mAngle, 0, 2000));
	bullet->SetBoundingShape(make_shared<BoundingSphere>(bullet->GetThisPtr(), 2.0f));
	bullet->SetShape(mBulletShape);
	// Add the new bullet to the game world
	mWorld->AddObject(bullet);

}

bool ComputerSpaceShip::CollisionTest(shared_ptr<GameObject> o)
{
	if (o->GetType() != GameObjectType("Asteroid")) return false;
	if (mBoundingShape.get() == NULL) return false;
	if (o->GetBoundingShape().get() == NULL) return false;
	return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void ComputerSpaceShip::OnCollision(const GameObjectList& objects)
{
	//this->SetVelocity(GLVector3f(0, 0, 0));
	//Thrust(0);
	//mWorld->FlagForRemoval(GetThisPtr());
}

void ComputerSpaceShip::AimAtPosition(const GLVector3f& targetPosition)
{
	// Calculate direction to target position
	GLVector3f direction = targetPosition - mPosition;

	// Calculate angle to align with direction
	float targetAngle = RAD2DEG * atan2(direction.y, direction.x);
	float angleDiff = targetAngle - mAngle;
	if (angleDiff > 180)
		angleDiff -= 360;
	else if (angleDiff < -180)
		angleDiff += 360;

	// Adjust rotation to align with direction
	Rotate(angleDiff);

	if (mFireCooldown <= 0) {
		Shoot(); // Fire a bullet
		mFireCooldown = mFireInterval; // Reset the cooldown timer
	}
	// Update the cooldown timer
	mFireCooldown -= 1.0f;
}

void ComputerSpaceShip::Navigation(const GameObjectList& asteroids, float maxThrust)
{
	// Find nearest asteroid
	float minDistance = std::numeric_limits<float>::max();
	GLVector3f targetDirection;
	for (const auto& asteroid : asteroids)
	{
		if (asteroid->GetType() != GameObjectType("Asteroid"))
			continue;

		GLVector3f direction = asteroid->GetPosition() - mPosition;
		float distance = direction.length();
		if (distance < minDistance)
		{
			minDistance = distance;
			targetDirection = direction;
		}
	}

	// Calculate angle to align with direction
	float targetAngle = RAD2DEG * atan2(targetDirection.y, targetDirection.x);
	float angleDiff = targetAngle - mAngle;
	if (angleDiff > 180) {
		angleDiff -= 500;
	}else if (angleDiff < -180) {
		angleDiff += 500;
	}

	// Adjust rotation to align with direction
	Rotate(angleDiff);

	//Shoot the asteroid
	if (minDistance < maxTargetingDistance) // Check if asteroid is within shooting range
	{
		// Aim at the predicted future position of the asteroid
		AimAtPosition(targetDirection);
	}

	// Thrust towards asteroid
	Thrust(maxThrust);
}
