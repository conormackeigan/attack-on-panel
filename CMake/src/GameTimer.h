#pragma once

#include "Entity.h"

class GameGrid;
class UIText;

class GameTimer : public Entity
{
public:
	GameTimer() = default;
	~GameTimer() = default;

	void Init() override;
	void OnDestroy() override;

	void StartTimer();
	void PauseTimer();

	UIText* timer_text;

protected:
	bool timer_on = false;

	// Total seconds elapsed since game started (smallest displayed measurement)
	unsigned int seconds;

	// number of frames until next second
	unsigned int tick_count;

public:
	unsigned int GetTime();
};

