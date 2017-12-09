#pragma once

#include "Animator.h"

class AnimatorSwapper : public Animator
{
public:
	AnimatorSwapper(Sprite* owner);
	~AnimatorSwapper();

	std::unordered_map<std::string, Animation> GetAnimationStates(Vector2i clip_size, Vector2i sheet_dimensions) override;
};

