extern "C" {
#include <SDL_image.h>
}

#include "TextureManager.h"
#include "RenderSystem.h"
#include <iostream>
#include "StandardTypes.h"
#include "Settings.h"

TextureManager::TextureAsset TextureManager::textureAssets[NUM_TEXTURES];


void TextureManager::Init()
{
	textureAssets[SWAPPER]		= TextureAsset("img/spr_swapper.tga", Vector2i(2, 1));
	textureAssets[PANEL_BLANK]	= TextureAsset("img/spr_panel_blank.tga", Vector2i(1, 1));
	textureAssets[PANEL_RED]	= TextureAsset("img/spr_panel_red.tga", Vector2i(3, 1));
	textureAssets[PANEL_GREEN]	= TextureAsset("img/spr_panel_green.tga", Vector2i(3, 1));
	textureAssets[PANEL_YELLOW] = TextureAsset("img/spr_panel_yellow.tga", Vector2i(3, 1));
	textureAssets[PANEL_CYAN]	= TextureAsset("img/spr_panel_cyan.tga", Vector2i(3, 1));
	textureAssets[PANEL_PURPLE] = TextureAsset("img/spr_panel_purple.tga", Vector2i(3, 1));
	textureAssets[PANEL_BLUE]	= TextureAsset("img/spr_panel_blue.tga", Vector2i(3, 1));

	textureAssets[PANEL_RED_DEAD]		= TextureAsset("img/spr_panel_red_dead.tga", Vector2i(1, 1));
	textureAssets[PANEL_GREEN_DEAD]		= TextureAsset("img/spr_panel_green_dead.tga", Vector2i(1, 1));
	textureAssets[PANEL_YELLOW_DEAD]	= TextureAsset("img/spr_panel_yellow_dead.tga", Vector2i(1, 1));
	textureAssets[PANEL_CYAN_BLUE_DEAD] = TextureAsset("img/spr_panel_cyan_blue_dead.tga", Vector2i(1, 1));
	textureAssets[PANEL_PURPLE_DEAD]	= TextureAsset("img/spr_panel_purple_dead.tga", Vector2i(1, 1));;

	textureAssets[COMBO_CHAIN_SPIRAL]	= TextureAsset("img/spr_combo_chain_spiral.tga", Vector2i(1, 1));
	textureAssets[COMBO_SIGN]			= TextureAsset("img/spr_combo_sign.tga", Vector2i(1, 1));
	textureAssets[CHAIN_SIGN]			= TextureAsset("img/spr_chain_sign.tga", Vector2i(1, 1));

	textureAssets[DIGIT_0] = TextureAsset("img/spr_digit_0.tga", Vector2i(1, 1));
	textureAssets[DIGIT_1] = TextureAsset("img/spr_digit_1.tga", Vector2i(1, 1));
	textureAssets[DIGIT_2] = TextureAsset("img/spr_digit_2.tga", Vector2i(1, 1));
	textureAssets[DIGIT_3] = TextureAsset("img/spr_digit_3.tga", Vector2i(1, 1));
	textureAssets[DIGIT_4] = TextureAsset("img/spr_digit_4.tga", Vector2i(1, 1));
	textureAssets[DIGIT_5] = TextureAsset("img/spr_digit_5.tga", Vector2i(1, 1));
	textureAssets[DIGIT_6] = TextureAsset("img/spr_digit_6.tga", Vector2i(1, 1));
	textureAssets[DIGIT_7] = TextureAsset("img/spr_digit_7.tga", Vector2i(1, 1));
	textureAssets[DIGIT_8] = TextureAsset("img/spr_digit_8.tga", Vector2i(1, 1));
	textureAssets[DIGIT_9] = TextureAsset("img/spr_digit_9.tga", Vector2i(1, 1));
	textureAssets[DIGIT_X] = TextureAsset("img/spr_digit_x.tga", Vector2i(1, 1));

	textureAssets[FADE_OVERLAY]	= TextureAsset("img/spr_black.tga", Vector2i(1, 1));
}

void TextureManager::Load()
{
	// For now just load every known texture file. Per-level loading/unloading later.
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		textureAssets[i].texture = IMG_LoadTexture(RenderSystem::Instance().renderer, textureAssets[i].filePath);
		
		ASSERT(textureAssets[i].texture != nullptr, "Error loading texture file: " << i);
		
		// Calculate and store this sheet's clip size (procedural grid for now)
		int width, height;
		SDL_QueryTexture(textureAssets[i].texture, nullptr, nullptr, &width, &height);

		Vector2i clipSize;

		if (textureAssets[i].sheetDimensions == Vector2i::zero)
		{	// If no dimensions are registered, entire texture is 1 clip
			clipSize = Vector2i(width, height);
		}
		else
		{
			clipSize = Vector2i(width / textureAssets[i].sheetDimensions.x, height / textureAssets[i].sheetDimensions.y);
		}

		textureAssets[i].clipSize = clipSize;
	}
}

void TextureManager::Unload()
{
	// For now just unload all loaded textures. Per-level loading/unloading later.
	for (int i = 0; i < NUM_TEXTURES; i++)
	{
		SDL_DestroyTexture(textureAssets[i].texture);
	}
}

void TextureManager::Shutdown()
{
	Unload();
}

SDL_Texture* TextureManager::GetTexture(TEXTURE_NAME name)
{
	if (name == NONE)
		return nullptr;

	ASSERT(textureAssets[name].texture != nullptr, "Texture " << name << " not found. " <<
		"Check TextureManager::textureFiles and resources folder for correct path");

	return textureAssets[name].texture;
}

const Vector2i& TextureManager::GetSpritesheetDimensions(TEXTURE_NAME name)
{
	return textureAssets[name].sheetDimensions;
}

const Vector2i& TextureManager::GetTextureClipSize(TEXTURE_NAME name)
{
	return textureAssets[name].clipSize;
}
