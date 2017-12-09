#pragma once

#include "Sprite.h"
#include "IPooled.h"
#include "StandardTypes.h"

class LogSpiralParticle : public IPooled<LogSpiralParticle>
{
public:
	LogSpiralParticle();
	~LogSpiralParticle();

	float angle = 720.f;
	float speed = 14.f;

	static const float a;
	static const float b;
	
	float starting_angle; // Degrees offset from x-axis

	Vector2f origin;

	void Set(Vector2f origin, float starting_angle);

	void Reset() override;
	void Update() override;
};

