#pragma once

extern "C" {
#include <SDL.h>
}

class IRenderable
{
public:
	virtual ~IRenderable() = default;

	virtual void Render(SDL_Renderer*) = 0;

	// Rendering drawOrder (higher number = drawn later)
protected:
	int drawOrder = 0;
	SDL_Texture* render_target = nullptr;

public:
	void SetDrawOrder(int draw_order);
	int GetDrawOrder() const;

	// Skip rendering this sprite if draw is false
	bool draw = true;
};

// Operators
bool operator<(const IRenderable& lhs, const IRenderable& rhs);
bool operator>(const IRenderable& lhs, const IRenderable& rhs);