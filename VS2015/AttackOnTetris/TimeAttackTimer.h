#pragma once

#include "GameTimer.h"

class TimeAttackTimer : public GameTimer
{
public:
	TimeAttackTimer() = default;
	~TimeAttackTimer() = default;

	void Init() override;
	void Update() override;
};

