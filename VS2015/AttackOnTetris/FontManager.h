#pragma once

extern "C" {
#include "SDL_FontCache.h"
}

#include <utility>

// TODO: refactor this spaghetti. Just cuz it works doesn't mean it's good.
class FontManager
{
public:
	// File + size pairings (corresponds 1:1 to fontsToLoad[])
	enum FONT_NAME
	{
		VERA_SE_24,
		VERA_SE_14
	};

	FontManager() = default;
	~FontManager() = default;

	void Init();
	void Load();
	void Unload();
	void Shutdown();

	static FC_Font* GetFont(FONT_NAME name);
	static FC_Font* GetDefaultFont();
	
private:
	// For looking up the font file
	enum FONT_FILE
	{
		VERA_SE,

		NUM_FONT_FILES
	};

	static const char* fontFiles[];
	static const std::pair<FONT_FILE, int> fontsToLoad[]; // font name and size
	static FC_Font* loadedFonts[];
};

