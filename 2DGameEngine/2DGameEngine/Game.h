#pragma once

// Include engine files
#include <GLFW/glfw3.h>
#include <iostream>
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Sprite.h"

// Include demo files
#include "BulletObject.h"
#include "EnemyObject.h"

//! Game class
class Game
{
private:
	Vector2 WindowSize;

	Sprite* PlayerSprite;
	Sprite* BulletSprite;
	Sprite* ExplosionSprite;
	Sprite* StarSprite;

	GameObject* PlayerObject;
	EnemyObject* Enemy;
	std::list<EnemyObject*> EnemyList;
	std::list<BulletObject*> BulletList;

	ParticleSystem* ThrusterParticleSystem;
	ParticleSystem* ExplosionParticleSystem;
	ParticleSystem* StarParticleSystem;
	std::list<ParticleSystem*> ExpirablePSList;
	std::list<ParticleSystem*> PSList;

	float MoveSpeed;
	Vector2 InputVector;
	float FireRate;
	float FireCooldown;

	// Enemy related
	float SpawnCount, SpawnRate, SpawnCooldown;

	//! Returns a random float based on the given parameters
	float RandomFloat(float MinRandomValue, float MaxRandomValue)
	{ return ((float)rand() / RAND_MAX) * (MaxRandomValue - MinRandomValue) + MinRandomValue; }

public:
	//! Takes in a Vector2, instantiates a Game instance and sets the window size variable
	Game(Vector2 DesiredWindowSize);
	~Game();

	//! Called at the start of runtime
	void Start();

	//! Takes in a GLFWwindow pointer and checks the input of the player
	void UpdateInput(GLFWwindow* window, float DeltaTime);
	//! Updates everything in the Game instance
	void Update(float DeltaTime);
	//! Draws everything in the Game instance
	void Draw();
};

