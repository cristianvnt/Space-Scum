#include "GameplayState.h"
#include "raylib.h"
#include "Config.h"

#include <string>
#include <random>

using namespace GAME;

GameplayState::GameplayState(const GameplaySettings& gps)
	: _gameplaySettings{ gps }, 
	_uiBounds{ { 0.f, 0.f, (float)gps.width, gps.margin }, { 0.f, (float)gps.height - gps.margin, (float)gps.width, gps.margin } }, 
	_player{ gps.playerSettings }
{
	_timer.StartTimer(Gameplay::SPAWN_TIME);
}

GameplayState::~GameplayState()
{
	for (const auto& e : _enemies)
		delete e;

	for (const auto& b : _bullets)
		delete b;
}

void GameplayState::Update(float dt)
{
	_timer.UpdateTimer();
	_player.GetCollisionTimer().UpdateTimer();
	CheckBounds();
	ProcessInput(dt);

	UpdateBullets(dt);
	UpdateEnemies(dt);
	CheckCollisions();
	CleanUp();

	if (_timer.TimerDone())
	{
		SpawnEnemies();
		_timer.StartTimer(Gameplay::SPAWN_TIME);
	}

	if (_player.GetCollisionTimer().TimerDone())
		_player.SetVulnerable(true);
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

	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		_player.Update({ 0, -1 }, dt);
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		_player.Update({ 0, 1 }, dt);
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
		_player.Update({ -1, 0 }, dt);
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		_player.Update({ 1, 0 }, dt);

	if (IsKeyPressed(KEY_SPACE))
		SpawnBullet();

	if (IsKeyPressed(KEY_ESCAPE))
		return StateType::MAIN_MENU;

	return StateType::NONE;
}

void GameplayState::CheckBounds()
{
	if ((_player.GetBody().x + _player.GetBody().width) >= (float)_gameplaySettings.width)
		_player.SetBodyVec({ (float)_gameplaySettings.width - _player.GetBody().width, _player.GetBody().y });
	if (_player.GetBody().x <= 0)
		_player.SetBodyVec({ 0.f, _player.GetBody().y });
	if ((_player.GetBody().y + _player.GetBody().height) >= (float)_gameplaySettings.height - Settings::MARGIN)
		_player.SetBodyVec({ _player.GetBody().x, (float)_gameplaySettings.height - Settings::MARGIN - _player.GetBody().height });
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

	_enemies.emplace_back(new Enemy{ {x, y, 50.f, 30.f}, 80.f, MAGENTA, true });
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
	_bullets.emplace_back(new Bullet{ {_player.GetBody().x, _player.GetBody().y, 10.f, 10.f}, 1000.f, RED, true});
}

void GameplayState::UpdateBullets(float dt)
{
	for (const auto& bullet : _bullets)
	{
		if (!bullet->IsActive())
			continue;

		bullet->Update(dt);

		if (bullet->GetBody().y + bullet->GetBody().height <= Settings::MARGIN)
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
