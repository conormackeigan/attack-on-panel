#pragma once

#include "Animator.h"

class AnimatorPanel : public Animator
{
public:
	AnimatorPanel(Sprite* owner);
	~AnimatorPanel();

	std::unordered_map<std::string, Animation> GetAnimationStates(Vector2i clip_size, Vector2i sheet_dimensions) override;
};

