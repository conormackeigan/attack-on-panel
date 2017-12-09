#include "AnimatorSwapper.h"

AnimatorSwapper::AnimatorSwapper(Sprite* owner) : Animator(owner, TextureManager::SWAPPER, Vector2i(74, 36), Vector2i(2, 1), GetAnimationStates(Vector2i(74, 36), Vector2i(2, 1)), "Idle")
{
}

AnimatorSwapper::~AnimatorSwapper()
{
}

std::unordered_map<std::string, Animation> AnimatorSwapper::GetAnimationStates(Vector2i clip_size, Vector2i sheet_dimensions)
{
	return 
	{
		{ "Idle", Animation({ 0, 1 }, sheet_dimensions, clip_size, 0.0125f) }
	};
}
