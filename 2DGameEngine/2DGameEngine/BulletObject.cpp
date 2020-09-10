#include "BulletObject.h"

BulletObject::BulletObject()
{
	Velocity = Vector2(0, 350);
}

BulletObject::BulletObject(Sprite* DesiredSprite)
	: GameObject(DesiredSprite)
{
	Velocity = Vector2(0, 350);
}

BulletObject::~BulletObject()
{
}

bool BulletObject::CheckBorders(const Vector2& GameBorder)
{
	if (GetPosition().X <= 0 - GetWorldScale().X / 2 || GetPosition().X >= GameBorder.X + GetWorldScale().X / 2 ||
		GetPosition().Y <= 0 - GetWorldScale().Y / 2 || GetPosition().Y >= GameBorder.Y + GetWorldScale().Y / 2)
		return true;
	else return false;
}

void BulletObject::Update(float DeltaTime)
{
	// Update the position
	Vector2 NewPosition = GetPosition() + (Velocity * DeltaTime);
	SetPosition(NewPosition);
}
