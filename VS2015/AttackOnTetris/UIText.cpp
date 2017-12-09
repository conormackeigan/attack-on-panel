#include "UIText.h"
#include <iostream>

void UIText::Init()
{
	// A font and color should always be set, so start with the defaults
	font = FontManager::GetDefaultFont();
	color = DEFAULT_TEXT_COLOR;
	text = "";
}

void UIText::Render(SDL_Renderer* renderer)
{
	FC_SetDefaultColor(font, color);
	FC_Draw(font, renderer, position.x, position.y, text.c_str());
}

void UIText::SetFont(FontManager::FONT_NAME newFont)
{
	font = FontManager::GetFont(newFont);
}

FC_Font* UIText::GetFont()
{
	return font;
}
