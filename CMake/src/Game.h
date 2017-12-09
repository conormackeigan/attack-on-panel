#pragma once

extern "C" {
#include <SDL.h>
}

#include "TextureManager.h"
#include "FontManager.h"

class Game
{
public:
	Game() = default;
	~Game() = default;

	// Asset managers
	TextureManager textureManager;
	FontManager fontManager;

	void Init(SDL_Window* window);
	void Update(SDL_Window* window);	
	void Render();
	void Shutdown();

	void Quit();
};

