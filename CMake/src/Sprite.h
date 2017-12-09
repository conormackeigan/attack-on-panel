#pragma once

#include "Entity.h"
#include "IRenderable.h"
#include "TextureManager.h"
#include "Vector2.h"
#include <vector>
#include "Animator.h"
#include <memory>
#include <cmath>

struct SDL_Renderer;


/**
	Purpose: Renderable game agent. Basic animation support via Animator class.
	Remarks: Sprites should be instantiated via EntityManager::Factory

	@author Conor MacKeigan 2017
*/

#define DEFAULT_SPRITE_SIZE		{ 48, 48 }
#define DEFAULT_NULL_TEX_COLOR	{ 0xC6, 0x1F, 0xCF, 0xFF } // Purple
#define COLOR_WHITE				{ 0xFF, 0xFF, 0xFF, 0xFF }

class Sprite : public Entity, public IRenderable
{
public:
	Sprite(Vector2f pos = Vector2f::zero);
	Sprite(TextureManager::TEXTURE_NAME texture_name, Vector2f pos = Vector2f::zero);

	virtual ~Sprite() = default;

	void SetActive(bool active) override;
	void SetActive(bool active, bool draw);

	bool IsSprite() override;

	// Where in the world is this sprite and carmen sandiego
	Vector2f position;

	// Clockwise rotation in degrees
	float rotation = 0.f; // for some reason if this isn't initialized to 0.f the angle gets borked (double cast?)

	// Sprite size in pixels
	Vector2i size;

	// Rectangle of current frame
	SDL_Rect src;

	// Get a pointer to the desired texture from TextureManager
	void SetTexture(TextureManager::TEXTURE_NAME textureName);

	// Draw the sprite to the screen
	virtual void Render(SDL_Renderer* renderer) override;

	// Rotate sprite clockwise in degrees
	void Rotate(float degrees);

	// For null textures and modulation/alpha of set textures
	SDL_Color color;

	// For color modulation and alpha blending
	SDL_BlendMode blendMode = SDL_BLENDMODE_BLEND;

	// This sprite's animation driver (if any)
	std::unique_ptr<Animator> animator = nullptr;

protected:
	SDL_Texture* texture = nullptr;

};
