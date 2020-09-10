#pragma once
#include "GameObject.h"
#include "BulletObject.h"
#include "ParticleSystem.h"


//! EnemyObject class, child of GameObject
class EnemyObject : public GameObject
{
private:
	Vector2 Velocity;

public:
	//! Instantiates EnemyObject with default values
	EnemyObject();
	//! Initialises a EnemyObject instance that takes in a Sprite pointer
	EnemyObject(Sprite* DesiredSprite);
	~EnemyObject();

	//! Takes in a Vector2 reference and sets the EnemyObject's velocity
	void SetVelocity(const Vector2& DesiredVelocity) { Velocity = DesiredVelocity; }

	//! Updates the EnemyObject
	void Update(float DeltaTime);
	//! Gets called when the EnemyObject comes into contact with a BulletObject, takes in a ParticleSystem reference
	void Destroy(ParticleSystem& DeathExplosion);
};

