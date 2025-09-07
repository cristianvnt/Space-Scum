#ifndef GAMEPLAY_STATE_H
#define GAMEPLAY_STATE_H
#include "GameState.h"
#include "utils/Timer.h"
#include "Entities/Player.h"
#include "Entities/Bullet.h"
#include "Entities/Enemy.h"

#include <vector>

struct UIBounds
{
	Rectangle topMargin{};
	Rectangle bottomMargin{};
};

struct GameplaySettings
{
	int width{};
	int height{};
	float margin{};
	PlayerSettings playerSettings{};
};

class GameplayState : public GameState
{
private:
	UIBounds _uiBounds{};
	Timer _gameTimer{};
	Player _player;
	GameplaySettings _gameplaySettings;

	std::vector<Enemy*> _enemies{};
	std::vector<Bullet*> _bullets{};

public:
	GameplayState(const GameplaySettings& gps);
	~GameplayState();

	StateType Update(float dt) override;
	void Draw() override;
	StateType ProcessInput(float dt) override;
	StateType GetStateType() const override { return StateType::GAMEPLAY; }

	void CheckBounds();

	Vector2 RandomEnemyPosition();
	void SpawnEnemies();
	void UpdateEnemies(float dt);

	void SpawnBullet();
	void UpdateBullets(float dt);

	void CheckCollisions();
	void CleanUp();
};

#endif
