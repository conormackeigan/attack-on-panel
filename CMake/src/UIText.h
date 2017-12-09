#pragma once

#include "UIElement.h"
#include <string>
#include "FontManager.h"

#define DEFAULT_TEXT_COLOR { 0xFF, 0xFF, 0xFF, 0xFF }

/**
	Purpose: UI Text data to be printed to the screen

	@author Conor MacKeigan 2017
*/
class UIText : public UIElement
{
public:
	UIText() : UIElement() { }
	~UIText() = default;

	void Init() override;
	void Render(SDL_Renderer* renderer) override;

	void SetFont(FontManager::FONT_NAME newFont);
	FC_Font* GetFont();

	std::string text;
	SDL_Color color;

private:
	FC_Font* font = nullptr;
};
