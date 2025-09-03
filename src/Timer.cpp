#include "Timer.h"
#include "raylib.h"

void Timer::StartTimer(float lifeTime)
{
	_lifeTime = lifeTime;
}

void Timer::UpdateTimer()
{
	if (_lifeTime < 0)
		return;

	_lifeTime -= GetFrameTime();
}

bool Timer::TimerDone() const
{
	return _lifeTime <= 0;
}