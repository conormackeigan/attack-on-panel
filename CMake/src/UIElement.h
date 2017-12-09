#pragma once

extern "C" {
#include <SDL.h>
}

#include "IRenderable.h"
#include "Vector2.h"

/**
	Purpose: Base class of all UI elements

	@author Conor MacKeigan 2017
*/

#define DEFAULT_NULL_TEX_COLOR	{ 0xC6, 0x1F, 0xCF, 0xFF } // Purple

class UIElement : public IRenderable
{
public:
	UIElement() { drawOrder = 1999999999; }
	virtual ~UIElement() = default;

	bool active = true;

	virtual void Render(SDL_Renderer* renderer) override = 0;
	
	virtual void Init() { }
	virtual void OnDestroy() { };

	Vector2f position;
	Vector2i size;
};
