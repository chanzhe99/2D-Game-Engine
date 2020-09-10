#include "Game.h"
#include <time.h>
#include <random>

#define OUT

Game::Game(Vector2 DesiredWindowSize)
{
	WindowSize = DesiredWindowSize;

	PlayerSprite = new Sprite("../media/Ship.bmp");
	BulletSprite = new Sprite("../media/Bullet.bmp");
	ExplosionSprite = new Sprite("../media/Explosion.bmp");
	StarSprite = new Sprite("../media/Star.bmp");

	PlayerObject = new GameObject(PlayerSprite);

	ThrusterParticleSystem = new ParticleSystem(ExplosionSprite, Vector2());
	StarParticleSystem = new ParticleSystem(StarSprite, Vector2(WindowSize.X / 2, WindowSize.Y));
}

Game::~Game()
{
}

void Game::Start()
{
	MoveSpeed = 200;
	PlayerObject->SetPosition(Vector2(WindowSize.X * 0.5f, WindowSize.Y * 0.2f));
	PlayerObject->SetBlendMode(BlendMode::Additive);
	InputVector = PlayerObject->GetPosition();
	FireRate = 0.1f;
	FireCooldown = 0;

	SpawnCount = 7;
	SpawnRate = 3;
	SpawnCooldown = SpawnRate;

	ThrusterParticleSystem->SetEmissionCount(5);
	ThrusterParticleSystem->SetEmissionRate(100);
	ThrusterParticleSystem->SetParticleScale(Vector2(0.5, 0.5));
	ThrusterParticleSystem->SetParticleColor(Color::Red);
	ThrusterParticleSystem->SetParticleVelocity(Vector2(0, -100));
	ThrusterParticleSystem->SetParticleGravity(Vector2(0, -500));
	ThrusterParticleSystem->SetParticleLifetime(0.5);
	ThrusterParticleSystem->SetAffectorRotation(270);
	ThrusterParticleSystem->SetAffectorScale(Vector2());
	ThrusterParticleSystem->SetAffectorColor(Color::Yellow);
	ThrusterParticleSystem->SetAffectorGravity(Vector2(0, -1000));
	PSList.push_back(ThrusterParticleSystem);

	StarParticleSystem->SetEmissionCount(3);
	StarParticleSystem->SetEmissionRate(1);
	StarParticleSystem->SetParticleColor(Color(1, 1, 1, 0.7));
	StarParticleSystem->SetParticleLifetime(10);
	StarParticleSystem->SetEmitterBox(Vector2(WindowSize.X, 10));
	PSList.push_back(StarParticleSystem);

}

void Game::UpdateInput(GLFWwindow* window, float DeltaTime)
{
	if (PlayerObject->GetPosition().X > 0 + PlayerObject->GetWorldScale().X / 2)
		if (glfwGetKey(window, GLFW_KEY_A) || glfwGetKey(window, GLFW_KEY_LEFT)) InputVector.X -= DeltaTime * MoveSpeed;
	if(PlayerObject->GetPosition().X < WindowSize.X - PlayerObject->GetWorldScale().X / 2)
		if (glfwGetKey(window, GLFW_KEY_D) || glfwGetKey(window, GLFW_KEY_RIGHT)) InputVector.X += DeltaTime * MoveSpeed;	
	if (PlayerObject->GetPosition().Y > 0 + PlayerObject->GetWorldScale().Y / 2)
		if (glfwGetKey(window, GLFW_KEY_S) || glfwGetKey(window, GLFW_KEY_DOWN)) InputVector.Y -= DeltaTime * MoveSpeed;
	if (PlayerObject->GetPosition().Y < WindowSize.Y - PlayerObject->GetWorldScale().Y / 2)
		if (glfwGetKey(window, GLFW_KEY_W) || glfwGetKey(window, GLFW_KEY_UP)) InputVector.Y += DeltaTime * MoveSpeed;

	if (FireCooldown > FireRate)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) || glfwGetKey(window, GLFW_KEY_SPACE))
		{
			BulletObject* NewBullet = new BulletObject(BulletSprite);
			Vector2 BulletSpawnPosition = Vector2(PlayerObject->GetPosition() + Vector2(0, PlayerObject->GetWorldScale().Y / 2));
			NewBullet->SetPosition(BulletSpawnPosition);
			NewBullet->SetBlendMode(BlendMode::Additive);

			BulletList.push_back(NewBullet);
			FireCooldown = 0;
		}
	}
	else FireCooldown += DeltaTime;
}

void Game::Update(float DeltaTime)
{
	std::cout << EnemyList.size() << std::endl;

	PlayerObject->SetPosition(InputVector);
	ThrusterParticleSystem->SetPosition(PlayerObject->GetPosition() - Vector2(0, PlayerObject->GetWorldScale().Y / 2));
	//ThrusterParticleSystem->Update(DeltaTime);
	
	float RandomSize = RandomFloat(0.25, 1);
	StarParticleSystem->SetParticleScale(Vector2(RandomSize, RandomSize));
	StarParticleSystem->SetParticleVelocity(Vector2(0, RandomFloat(-100, -25)));

	if (SpawnCooldown >= SpawnRate)
	{
		for (int i = 0; i < SpawnCount; i++)
		{
			EnemyObject* NewEnemy = new EnemyObject(PlayerSprite);
			NewEnemy->SetPosition(Vector2(WindowSize.X / SpawnCount * i + NewEnemy->GetWorldScale().X * (rand() % (int)SpawnCount + 1), WindowSize.Y + NewEnemy->GetWorldScale().Y / 2));
			NewEnemy->SetRotation(180);
			NewEnemy->SetScale(Vector2(2, 2));
			NewEnemy->SetColor(Color::Red);
			NewEnemy->SetBlendMode(BlendMode::Additive);
			NewEnemy->SetVelocity(Vector2(0, RandomFloat(-100, -50)));

			EnemyList.push_back(NewEnemy);
		}
		SpawnCooldown = 0;
	}
	else SpawnCooldown += DeltaTime;

	// Iterate throught bullet list
	std::list<BulletObject*>::iterator BulletIterator = BulletList.begin();
	while (BulletIterator != BulletList.end())
	{
		BulletObject* TempBullet = *BulletIterator;
		TempBullet->Update(DeltaTime);
		if (TempBullet->CheckBorders(WindowSize))
		{
			delete TempBullet;
			BulletIterator = BulletList.erase(BulletIterator);
		}
		else
		{
			// Iterate through enemy list to check for collisions
			std::list<EnemyObject*>::iterator EnemyIterator = EnemyList.begin();
			while (EnemyIterator != EnemyList.end())
			{
				EnemyObject* TempEnemy = *EnemyIterator;
				if (TempEnemy->CheckCollision(*TempBullet))
				{
					// Create the enemy explosion particle
					ExplosionParticleSystem = new ParticleSystem(ExplosionSprite, TempEnemy->GetPosition());
					TempEnemy->Destroy(OUT *ExplosionParticleSystem);
					ExpirablePSList.push_back(ExplosionParticleSystem);
					EnemyIterator = EnemyList.erase(EnemyIterator);
				}
				else EnemyIterator++;
			}
			BulletIterator++;
		}
	}

	std::list<EnemyObject*>::iterator EnemyIterator = EnemyList.begin();
	while (EnemyIterator != EnemyList.end())
	{
		EnemyObject* TempEnemy = *EnemyIterator;
		TempEnemy->Update(DeltaTime);
		if (TempEnemy->GetPosition().Y <= 0 - TempEnemy->GetWorldScale().Y / 2)
		{
			delete TempEnemy;
			EnemyIterator = EnemyList.erase(EnemyIterator);
		}
		else EnemyIterator++;
	}

	// Iterate through particle system list
	std::list<ParticleSystem*>::iterator ExpirablePSIterator = ExpirablePSList.begin();
	while (ExpirablePSIterator != ExpirablePSList.end())
	{
		ParticleSystem* TempExpirablePS = *ExpirablePSIterator;
		TempExpirablePS->Update(DeltaTime);
		if (TempExpirablePS->GetParticleSystemCurrentLifetime() >= TempExpirablePS->GetParticleSystemMaxLifetime())
		{
			delete TempExpirablePS;
			ExpirablePSIterator = ExpirablePSList.erase(ExpirablePSIterator);
		}
		else ExpirablePSIterator++;
	}

	std::list<ParticleSystem*>::iterator PSIterator = PSList.begin();
	while (PSIterator != PSList.end())
	{
		ParticleSystem* TempPS = *PSIterator;
		TempPS->Update(DeltaTime);
		PSIterator++;
	}
}

void Game::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	std::list<BulletObject*>::iterator BulletIterator = BulletList.begin();
	while (BulletIterator != BulletList.end())
	{
		BulletObject* TempBullet = *BulletIterator;
		TempBullet->Draw();
		BulletIterator++;
	}

	std::list<EnemyObject*>::iterator EnemyIterator = EnemyList.begin();
	while (EnemyIterator != EnemyList.end())
	{
		EnemyObject* TempEnemy = *EnemyIterator;
		TempEnemy->Draw();
		EnemyIterator++;
	}

	std::list<ParticleSystem*>::iterator ExpirablePSIterator = ExpirablePSList.begin();
	while (ExpirablePSIterator != ExpirablePSList.end())
	{
		ParticleSystem* TempExpirablePS = *ExpirablePSIterator;
		TempExpirablePS->Draw();
		ExpirablePSIterator++;
	}

	std::list<ParticleSystem*>::iterator PSIterator = PSList.begin();
	while (PSIterator != PSList.end())
	{
		ParticleSystem* TempPS = *PSIterator;
		TempPS->Draw();
		PSIterator++;
	}

	PlayerObject->Draw();
	//ThrusterParticleSystem->Draw();
}
