#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
	float _lifeTime = -1.f;

public:
	Timer() = default;

	void StartTimer(float lifeTime);
	void UpdateTimer();
	bool TimerDone() const;
};

#endif
