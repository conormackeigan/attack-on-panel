#pragma once

#include "Sprite.h"
#include "ScreenFader.h"

class ScreenFade : public Sprite
{
public:
	ScreenFade();
	~ScreenFade() = default;

	void Update() override;

	ScreenFader::Fade fade;
};

