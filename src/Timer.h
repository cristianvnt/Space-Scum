#ifndef TIMER_H
#define TIMER_H

class Timer
{
private:
	float _lifeTime;

public:
	Timer() = default;

	void StartTimer(float lifeTime);
	void UpdateTimer();
	bool TimerDone() const;
};

#endif
