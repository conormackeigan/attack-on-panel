#pragma once

#include "UIElement.h"
#include "RenderSystem.h"
#include <memory>
#include <vector>
#include "Vector2.h"

/**
	Purpose: Provides a factory for and maintains ownership of all UI elements

	@author Conor MacKeigan 2017
*/
class UIManager
{
public:
	static UIManager& Instance();

	UIManager(const UIManager&)		 = delete;
	void operator=(const UIManager&) = delete;

	void Init();

	//-----------------------------------------------------
	//	Factory
	//-----------------------------------------------------
	template <typename T>
	T* Factory(Vector2f position = Vector2f::zero)
	{
		UIElements.emplace_back(std::make_unique<T>());

		// Register UI Element with RenderSystem to be rendered
		UIElement* newElement = UIElements.back().get();
		RenderSystem::Instance().RegisterRenderable(newElement);

		newElement->Init();
		newElement->position = position;

		return static_cast<T*>(newElement);
	}

	// Unfactory
	void DestroyUIElement(UIElement* ui_element);

private:
	UIManager() = default;

	std::vector<std::unique_ptr<UIElement>> UIElements;
};

