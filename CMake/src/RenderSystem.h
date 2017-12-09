#pragma once

extern "C" {
#include <SDL.h>
}

#include <vector>
#include <memory>

class Sprite;
class Animator;
class UIElement;
class IRenderable;

class RenderSystem
{
public:
	static RenderSystem& Instance();

	RenderSystem(const RenderSystem&)   = delete;
	void operator=(const RenderSystem&) = delete;

	SDL_Renderer* renderer;

	void Init(SDL_Window* window);
	void Render();
	void Shutdown();

	void RegisterRenderable(IRenderable* renderable);
	void UnregisterRenderable(IRenderable* renderable);

	void SortRenderables();

	void RegisterAnimator(Animator* animator);
	void UnregisterAnimator(Animator* animator);

//private:
	RenderSystem() = default;

	std::vector<Animator*> animators;
	std::vector<IRenderable*> renderables;
};

