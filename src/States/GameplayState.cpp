#include "GameplayState.h"
#include "raylib.h"

#include <string>
#include <random>

GameplayState::GameplayState(const GameplaySettings& gps)
	: _gameplaySettings{ gps },
	_uiBounds{ { 0.f, 0.f, (float)gps.width, gps.margin }, { 0.f, (float)gps.height - gps.margin, (float)gps.width, gps.margin } },
	_player{ gps.playerSettings }
{
	_gameTimer.StartTimer(gps.enemySettings.spawnTime);
}

GameplayState::~GameplayState()
{
	for (const auto& e : _enemies)
		delete e;

	for (const auto& b : _bullets)
		delete b;
}

StateType GameplayState::Update(float dt)
{
	StateType state = ProcessInput(dt);
	if (state != StateType::NONE)
		return state;

	_gameTimer.UpdateTimer();
	_player.GetCollisionTimer().UpdateTimer();
	CheckBounds();

	_player.Update(dt);

	if (IsKeyPressed(KEY_SPACE))
		SpawnBullet();

	UpdateBullets(dt);
	UpdateEnemies(dt);
	CheckCollisions();
	CleanUp();

	if (_gameTimer.TimerDone())
	{
		SpawnEnemies();
		_gameTimer.StartTimer(_gameplaySettings.enemySettings.spawnTime);
	}

	if (_player.GetCollisionTimer().TimerDone())
		_player.SetVulnerable(true);

	return StateType::NONE;
}

void GameplayState::Draw()
{
	_player.Draw();

	for (const auto& bullet : _bullets)
		bullet->Draw();

	for (const auto& enemy : _enemies)
		enemy->Draw();

	DrawRectangleRec(_uiBounds.topMargin, LIGHTGRAY);

	for (size_t i = 0; i < _player.GetLives(); ++i)
		DrawRectangleRec({ _player.GetLife().x - i * 40, _player.GetLife().y, _player.GetLife().width, _player.GetLife().height }, GREEN);

	DrawRectangleRec(_uiBounds.bottomMargin, LIGHTGRAY);
	DrawText(("Score: " + std::to_string(_player.GetScore())).c_str(), 20, _gameplaySettings.height - 50, 50, BLACK);

	DrawFPS(10, 10);
}

StateType GameplayState::ProcessInput(float dt)
{
	if (!_player.GetLives())
		return StateType::GAME_OVER;

	_player.DefaultVelocity();

	Vector2 velocity = { 0.f, 0.f };

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		velocity.y -= 1.f;
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		velocity.y += 1.f;
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		velocity.x -= 1.f;
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		velocity.x += 1.f;

	_player.SetVelocity({ velocity.x * _gameplaySettings.playerSettings.speed, velocity.y * _gameplaySettings.playerSettings.speed });

	if (IsKeyPressed(KEY_ESCAPE))
		return StateType::MAIN_MENU;

	if (IsKeyPressed(KEY_P))
		return StateType::PAUSE;

	return StateType::NONE;
}

void GameplayState::CheckBounds()
{
	if ((_player.GetBody().x + _player.GetBody().width) >= (float)_gameplaySettings.width)
		_player.SetBodyVec({ (float)_gameplaySettings.width - _player.GetBody().width, _player.GetBody().y });
	if (_player.GetBody().x <= 0)
		_player.SetBodyVec({ 0.f, _player.GetBody().y });
	if ((_player.GetBody().y + _player.GetBody().height) >= (float)_gameplaySettings.height - _gameplaySettings.margin)
		_player.SetBodyVec({ _player.GetBody().x, (float)_gameplaySettings.height - _gameplaySettings.margin - _player.GetBody().height });
	if ((_player.GetBody().y) <= (float)_gameplaySettings.height / 2.f)
		_player.SetBodyVec({ _player.GetBody().x, (float)_gameplaySettings.height / 2.f });
}

Vector2 GameplayState::RandomEnemyPosition()
{
	std::random_device dev;
	static std::mt19937 rng(dev());

	std::uniform_real_distribution<float> distX(50.f, (float)_gameplaySettings.width - 50.f);
	std::uniform_real_distribution<float> distY(-50.f, 0.f);

	return { distX(rng), distY(rng) };
}

void GameplayState::SpawnEnemies()
{
	bool validEnemyPos = false;
	float x{};
	float y{};

	while (!validEnemyPos)
	{
		x = RandomEnemyPosition().x;
		y = RandomEnemyPosition().y;
		Rectangle tempEnemy{ x, y, 50.f, 30.f };
		validEnemyPos = true;

		for (const auto& e : _enemies)
		{
			if (CheckCollisionRecs(e->GetBody(), tempEnemy))
			{
				validEnemyPos = false;
				break;
			}
		}
	}

	const EnemySettings& es = _gameplaySettings.enemySettings;

	_enemies.emplace_back(new Enemy{ EnemySettings{}
		.SetBody({ x, y, es.body.width, es.body.height })
		.SetVelocity({0, 1}, es.speed)
		.SetColor(MAGENTA)
		.SetActive(true)
	});
}

void GameplayState::UpdateEnemies(float dt)
{
	for (const auto& enemy : _enemies)
	{
		if (!enemy->IsActive())
			continue;

		enemy->Update(dt);

		if (!_player.GetLives())
			return;

		if (CheckCollisionRecs(enemy->GetBody(), _player.GetBody()) && _player.GetLives())
		{
			if (_player.IsVulnerable())
			{
				_player.SetLives(_player.GetLives() - 1);
				_player.GetCollisionTimer().StartTimer(0.5f);
				_player.SetVulnerable(false);
			}
		}

		if (enemy->GetBody().y >= _uiBounds.bottomMargin.y && _player.GetLives())
		{
			enemy->SetActive(false);
			_player.SetLives(_player.GetLives() - 1);
		}
	}
}

void GameplayState::SpawnBullet()
{
	Vector2 pVel = _player.GetVelocity();
	Vector2 bVel = { 0, -_gameplaySettings.bulletSettings.speed };
	Vector2 finalVelocity = { pVel.x + bVel.x, pVel.y + bVel.y };

	_bullets.emplace_back(new Bullet{ BulletSettings{}
		.SetBody({_player.GetBody().x, _player.GetBody().y, 10.f, 15.f})
		.SetVelocity(finalVelocity)
		.SetColor(RED)
		.SetActive(true)
	});
}

void GameplayState::UpdateBullets(float dt)
{
	for (const auto& bullet : _bullets)
	{
		if (!bullet->IsActive())
			continue;

		bullet->Update(dt);

		if (bullet->GetBody().y + bullet->GetBody().height <= _gameplaySettings.margin)
			bullet->SetActive(false);
	}
}

void GameplayState::CheckCollisions()
{
	for (const auto& enemy : _enemies)
	{
		for (const auto& bullet : _bullets)
		{
			if (enemy->IsActive() && bullet->IsActive() && CheckCollisionRecs(enemy->GetBody(), bullet->GetBody()))
			{
				enemy->SetActive(false);
				bullet->SetActive(false);
				_player.SetScore(_player.GetScore() + 10);
			}
		}
	}
}

void GameplayState::CleanUp()
{
	_bullets.erase(std::remove_if(_bullets.begin(), _bullets.end(), [](const Bullet* b) {
		if (!b->IsActive())
		{
			delete b;
			return true;
		}
		return false;
		}), _bullets.end());

	_enemies.erase(std::remove_if(_enemies.begin(), _enemies.end(), [](const Enemy* e) {
		if (!e->IsActive())
		{
			delete e;
			return true;
		}
		return false;
		}), _enemies.end());

}
