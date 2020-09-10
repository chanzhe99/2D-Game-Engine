#include "EnemyObject.h"

EnemyObject::EnemyObject()
{
}

EnemyObject::EnemyObject(Sprite* DesiredSprite) : GameObject(DesiredSprite)
{
	
}

EnemyObject::~EnemyObject()
{
}

void EnemyObject::Update(float DeltaTime)
{
	SetPosition(GetPosition() + Velocity * DeltaTime);
}

void EnemyObject::Destroy(ParticleSystem& DeathExplosion)
{
	// Set particle system attributes
	DeathExplosion.SetMaxLifetime(RandomFloat(2, 5));
	DeathExplosion.SetEmissionCount(RandomFloat(10, 30));
	DeathExplosion.SetEmissionRate(1 / DeathExplosion.GetParticleSystemMaxLifetime() + RandomFloat(0, 20));

	// Set particle attributes
	float RandomScale = RandomFloat(1, 2);
	DeathExplosion.SetParticleRotation(RandomFloat(0, 360));
	DeathExplosion.SetParticleScale(Vector2(RandomScale, RandomScale));
	DeathExplosion.SetParticleColor(Color(RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1)));
	DeathExplosion.SetParticleGravity(Vector2(RandomFloat(-1000, 1000), RandomFloat(-1000, 1000)));
	DeathExplosion.SetParticleLifetime(DeathExplosion.GetParticleSystemMaxLifetime());

	// Set affector attributes
	DeathExplosion.SetAffectorColor(Color(RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1), RandomFloat(0, 1)));
	DeathExplosion.SetAffectorScale(Vector2());
	DeathExplosion.SetAffectorRotation(RandomFloat(0, 360));
	DeathExplosion.SetAffectorGravity(Vector2(RandomFloat(-1000, 1000), RandomFloat(-1000, 1000)));

	int Rand = rand() % 4;
	switch (Rand)
	{
	case 0:
		DeathExplosion.SetEmitterPoint();
		break;
	case 1:
		DeathExplosion.SetEmitterBox(Vector2(RandomFloat(0, 100), RandomFloat(0, 100)));
		break;
	case 2:
		DeathExplosion.SetEmitterCircle(RandomFloat(0, 100));
		break;
	case 3:
		DeathExplosion.SetEmitterRing(RandomFloat(0, 100), RandomFloat(0, 100));
		break;
	}

	delete this;
}
