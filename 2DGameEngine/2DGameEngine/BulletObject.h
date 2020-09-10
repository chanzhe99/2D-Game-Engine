#pragma once
#include "GameObject.h"
#include "ParticleSystem.h"


//! BulletObject class, child of GameObject
class BulletObject : public GameObject
{
private:
	Vector2 Velocity;

public:
	//! Instantiates BulletObject with default values
	BulletObject();
	//! Initialises a EnemyObject instance that takes in a Sprite pointer
	BulletObject(Sprite* DesiredSprite);
	~BulletObject();

	//! Returns true if the bullet goes off screen
	bool CheckBorders(const Vector2& GameBorder);
	//! Updates the BulletObject
	void Update(float DeltaTime);
};

