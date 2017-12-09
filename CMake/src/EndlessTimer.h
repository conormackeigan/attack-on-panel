#pragma once

#include "GameTimer.h"

class EndlessTimer : public GameTimer
{
public:
	EndlessTimer() = default;
	~EndlessTimer() = default;

	void Init() override;
	void Update() override;
};

