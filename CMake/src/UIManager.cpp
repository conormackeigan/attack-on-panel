#include "UIManager.h"
#include "RenderSystem.h"
#include <iostream>
#include <algorithm>

// Element includes
#include "UIText.h"

#define render_system RenderSystem::Instance()

UIManager& UIManager::Instance()
{
	static UIManager instance;

	return instance;
}

void UIManager::Init()
{
	UIElements.reserve(32);
}
	
void UIManager::DestroyUIElement(UIElement* ui_element)
{
	ui_element->OnDestroy();

	// Remove weak pointer in RenderSystem
	render_system.UnregisterRenderable(ui_element);

	// Goodbye (memory managed by unique_ptr)
	UIElements.erase(std::remove_if(UIElements.begin(),
									UIElements.end(),
									[ui_element](const std::unique_ptr<UIElement>& e) { return e.get() == ui_element; }), UIElements.end());
}
