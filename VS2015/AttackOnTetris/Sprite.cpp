extern "C" {
#include <SDL.h>
}

#include "Sprite.h"
#include <iostream>

Sprite::Sprite(Vector2f pos) : position(pos)
{
	// Initialize a default sprite with null texture
	size  =	DEFAULT_SPRITE_SIZE;
	color = DEFAULT_NULL_TEX_COLOR;
}

Sprite::Sprite(TextureManager::TEXTURE_NAME texture_name, Vector2f pos) : position(pos)
{
	SetTexture(texture_name);
}

void Sprite::SetActive(bool active)
{
	this->active = active;
	draw = active;
}

void Sprite::SetActive(bool active, bool draw)
{
	this->active = active;
	this->draw = draw;
}

bool Sprite::IsSprite()
{
	return true;
}

// TODO: decouple clip size from sprite entity for a more modular animation system
void Sprite::SetTexture(TextureManager::TEXTURE_NAME texture_name)
{
	texture = TextureManager::GetTexture(texture_name);

	size = Vector2i(TextureManager::GetTextureClipSize(texture_name));

	// If assigning a texture to a null texture sprite, set color to opaque white
	if (texture != nullptr)
	{
		color = COLOR_WHITE;
	}
	else
	{
		color = DEFAULT_NULL_TEX_COLOR;
	}
	
	// Default src rectangle to first clip
	src = { 0, 0, size.x, size.y };
}

void Sprite::Render(SDL_Renderer* renderer)
{
	// Set appropriate blend mode and alpha & color modulation
	SDL_SetTextureBlendMode(texture, blendMode);
	
	SDL_SetTextureAlphaMod(texture, color.a);
	SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

	SDL_Rect dst = { (int)position.x, (int)position.y, size.x, size.y }; 

	// This sprite has a texture set, render a portion of it
	if (texture != nullptr)
	{	
		if (rotation == 0.f)
		{
			SDL_RenderCopy(renderer, texture, &src, &dst);
		}
		else
		{
			SDL_RenderCopyEx(renderer, texture, &src, &dst, rotation, nullptr, SDL_FLIP_NONE);
		}
	}
	// No texture set, draw a coloured rectangle
	else
	{	
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &dst);	
	}
}

void Sprite::Rotate(float degrees)
{
	rotation = fmod(rotation + degrees, 360.f);
}
