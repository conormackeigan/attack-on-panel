#pragma once

#include "IPooled.h"

class Digit;

class ChainSign : public IPooled<ChainSign>
{
public:
	ChainSign();

	static unsigned int rise_table[41];

	unsigned int timer = 0;
	float starting_height;

	std::vector<Digit*> digits; // Digit sprites

	void Set(Vector2f pos, unsigned int value);

	void Update() override;
	void Reset() override;
	void Recycle() override;
};

