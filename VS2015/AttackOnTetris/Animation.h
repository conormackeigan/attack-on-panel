#pragma once

extern "C" {
#include <SDL.h>
}

#include <vector>
#include "Vector2.h"

class Animation
{
public:
	/********************************************************
	Texture grid is indexed left-right, top-bottom. Eg:

	0 1 2 3
	4 5 6 7
	8 9	etc

	********************************************************/

	// For calculating srcRects with clip index, spritesheet dimensions and individual clip size information
	Animation(std::vector<int> clips, const Vector2i& sheetDimensions, const Vector2i& clipSize, float speed = 1.f, bool loop = true);

	// For when srcRects are already calculated
	Animation(std::vector<SDL_Rect> srcRects, float speed = 1.f, bool loop = true);

	std::vector<SDL_Rect> srcRects;
	float speed = 1.f; // 1.0 = 60fps, 0.5 = 30fps, 0.25 = 15fps, etc
	bool loop = true;
};

