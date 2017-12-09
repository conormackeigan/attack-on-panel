#pragma once

extern "C" {
#include <SDL.h>
}

#include <unordered_map>
#include <vector>
#include "Vector2.h"


/**
	Purpose: Load and handle ownership and distribution of texture assets.
			 Registers import settings that would normally be parsed from meta files.

	TODO: per-level loading/unloading, more modularity

	@author Conor MacKeigan 2017
*/

class TextureManager
{
public:
	// Maps to textureAssets[TEXTURE_NAME]
	// Todo: have sprites get textures by filename rather than hard-coded enum value.
	//		 Meta data will have to be parsed from an external file.
	enum TEXTURE_NAME
	{
		NONE = -1,

		SWAPPER,

		// Panels
		PANEL_BLANK,
		PANEL_RED,
		PANEL_GREEN,
		PANEL_YELLOW,
		PANEL_CYAN,
		PANEL_PURPLE,
		PANEL_BLUE,

		PANEL_RED_DEAD,
		PANEL_GREEN_DEAD,
		PANEL_YELLOW_DEAD,
		PANEL_CYAN_BLUE_DEAD,
		PANEL_PURPLE_DEAD,

		COMBO_CHAIN_SPIRAL,
		COMBO_SIGN,
		CHAIN_SIGN,

		DIGIT_0,
		DIGIT_1,
		DIGIT_2,
		DIGIT_3,
		DIGIT_4,
		DIGIT_5,
		DIGIT_6,
		DIGIT_7,
		DIGIT_8,
		DIGIT_9,
		DIGIT_X, // "x" multiplier prefix. Not actually a digit...

		// For fade out/in screen transitions
		FADE_OVERLAY,

		NUM_TEXTURES
	};

	// Takes a filepath and spritesheet dimensions as input, then loads and slices texture
	// todo: atlas map so spritesheets don't have to be an even grid
	struct TextureAsset
	{
		TextureAsset() = default;

		// Note: clipSize is calculated from sheetDimensions, and texture is loaded
		TextureAsset(const char* filePath, const Vector2i& sheetDimensions) : 
			filePath(filePath), sheetDimensions(sheetDimensions) { }
	
		const char* filePath;	

		// Meta data
		Vector2i sheetDimensions;
		Vector2i clipSize;

		// The loaded texture asset
		SDL_Texture* texture = nullptr;
	};

	TextureManager() = default;
	~TextureManager() = default;

	// Populate map of texture filenames yay hard-coding
	void Init();

	// Load textures from resources folder
	void Load();

	// Deallocate memory and remove references to loaded textures
	void Unload();

	// Game closed
	void Shutdown();

	// Distribute textures to requesting entities
	static SDL_Texture* GetTexture(TEXTURE_NAME name);

	// Retrive spritesheet dimensions for a texture
	static const Vector2i& GetSpritesheetDimensions(TEXTURE_NAME name);

	// Retrieve individual clip size for a texture sheet
	static const Vector2i& GetTextureClipSize(TEXTURE_NAME name);

private:
	static TextureAsset textureAssets[];
};