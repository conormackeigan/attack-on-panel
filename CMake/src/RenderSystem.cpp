#include "RenderSystem.h"
#include "Settings.h"

extern "C" {
#include <SDL_image.h>
}

#include <algorithm>
#include "Sprite.h"
#include "Animator.h"
#include "UIElement.h"
#include "Utilities.h"

RenderSystem& RenderSystem::Instance()
{
	static RenderSystem instance;

	return instance;
}

void RenderSystem::Init(SDL_Window* window)
{
	// Initialize SDL_Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);// | SDL_RENDERER_PRESENTVSYNC);

	// Reserve memory for weak pointers
	animators.reserve(256);
	renderables.reserve(256);
}

void RenderSystem::Render()
{
	// Clear screen
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(renderer);

	// Update all animators before rendering sprites
	for (std::vector<Animator*>::iterator it = animators.begin(); it != animators.end(); ++it)
	{
		if ((*it)->owner->IsActive()) // Branching D:
			(*it)->Update();
	}

	// Render all the renderables
	for (std::vector<IRenderable*>::iterator it = renderables.begin(); it != renderables.end(); ++it)
	{
		if ((*it)->draw) // Branching D:
			(*it)->Render(renderer);
	}

	SDL_RenderPresent(renderer);
}

void RenderSystem::Shutdown()
{
	SDL_DestroyRenderer(renderer);
}

void RenderSystem::RegisterRenderable(IRenderable* renderable)
{
	renderables.push_back(renderable);
	SortRenderables();
}

void RenderSystem::UnregisterRenderable(IRenderable* renderable)
{
	renderables.erase(std::remove(renderables.begin(), renderables.end(), renderable), renderables.end());
}

void RenderSystem::SortRenderables()
{
	// Insertion sort sprites by draw order
	insertionSort<IRenderable>(renderables);
}

void RenderSystem::RegisterAnimator(Animator* animator)
{
	if (animator != nullptr)
		animators.push_back(animator);
}

void RenderSystem::UnregisterAnimator(Animator* animator)
{
	if (animator != nullptr)
		animators.erase(std::remove(animators.begin(), animators.end(), animator), animators.end());
}
