#pragma once

extern "C" {
#include <SDL.h>
}

struct ColorCode
{
public:
	ColorCode() = delete;
	~ColorCode() = delete;

	static const SDL_Color WHITE;
	static const SDL_Color BLACK;
	static const SDL_Color BROWN;
	static const SDL_Color RED;
	static const SDL_Color GREEN;
	static const SDL_Color BLUE;
	static const SDL_Color CYAN;
	static const SDL_Color YELLOW;
	static const SDL_Color ORANGE;
};

