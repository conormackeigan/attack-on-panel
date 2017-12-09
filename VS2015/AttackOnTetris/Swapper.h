#pragma once

#include "Sprite.h"

class Swapper : public Sprite
{
public:
	Swapper();
	~Swapper();

	void Init() override;
	void Update() override;

	Vector2i grid_pos;
};

