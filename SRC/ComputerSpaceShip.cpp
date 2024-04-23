#include "GameUtil.h"
#include "GameWorld.h"
#include "Bullet.h"
#include "ComputerSpaceShip.h"
#include "BoundingSphere.h"
#include "GameObject.h"

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
	//mWorld->FlagForRemoval(GetThisPtr());
}

void ComputerSpaceShip::Remove() {
	mWorld->FlagForRemoval(GetThisPtr());
}
