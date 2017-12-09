#include "FontManager.h"
#include "RenderSystem.h"
#include "StandardTypes.h"

// Populate in Init()
const char* FontManager::fontFiles[NUM_FONT_FILES];

// All used combinations of font type & size
const std::pair<FontManager::FONT_FILE, int> FontManager::fontsToLoad[] = 
{
	{ FontManager::FONT_FILE::VERA_SE, 24 },
	{ FontManager::FONT_FILE::VERA_SE, 14 }
};

// Usable fonts to be distributed to UI elements
FC_Font* FontManager::loadedFonts[sizeof(fontsToLoad) / sizeof(*fontsToLoad)];


void FontManager::Init()
{
	// Set font file paths
	fontFiles[VERA_SE] = "fonts/VeraSe.ttf";
}

void FontManager::Load()
{
	// Load all fonts into memory
	int n = sizeof(fontsToLoad) / sizeof(*fontsToLoad);
	for (int i = 0; i < n; i++)
	{
		FontManager::FONT_FILE font_name = fontsToLoad[i].first;
		int font_size = fontsToLoad[i].second;

		FC_Font* font = FC_CreateFont();
		FC_LoadFont(font, RenderSystem::Instance().renderer, fontFiles[font_name], font_size, FC_MakeColor(0x00, 0x00, 0x00, 0xFF), TTF_STYLE_NORMAL);

		loadedFonts[i] = font;

		ASSERT(loadedFonts[i] != nullptr, "Error loading font ID " << i << std::endl);
	}
}

void FontManager::Unload()
{
	// Unload all fonts from memory
	int n = sizeof(loadedFonts) / sizeof(*loadedFonts);
	for (int i = 0; i < n; i++)
	{
		FC_FreeFont(loadedFonts[i]);
	}
}

void FontManager::Shutdown()
{
	// Unload all loaded fonts
	Unload();

	// SDL_FontCache calls TTF_Init() but not Quit
	TTF_Quit();
}

FC_Font* FontManager::GetFont(FONT_NAME name)
{
	return loadedFonts[name];
}

FC_Font* FontManager::GetDefaultFont()
{
	return loadedFonts[0];
}
