#pragma once

#include <unordered_map>
#include <string>
#include "Animation.h"
#include "TextureManager.h"

class Sprite;

/**
	Purpose: Base class of all animators, which contain entity-specific animation data and drive the states
	NOTE: makes use of the Curiously Recurring Template Pattern

	@author Conor MacKeigan 2017
*/
class Animator
{
public:
	Animator(Sprite* owner, TextureManager::TEXTURE_NAME texture_name, Vector2i clip_size, Vector2i sheet_dimensions, std::unordered_map<std::string, Animation> animation_states, std::string default_state);
	virtual ~Animator() = default;

	std::unordered_map<std::string, Animation> animation_states;
	std::string default_state;

protected:
	TextureManager::TEXTURE_NAME texture_name;
	Vector2i sheet_dimensions;
	Vector2i clip_size;

	Animation* curr_animation;

	unsigned int ticks = 0;
	unsigned int curr_frame = 0;
	int ticks_per_frame = 0;

public:
	// Drive the animation
	void Update();

	// Depends on derived animation states
	void SetState(std::string state);

	virtual std::unordered_map<std::string, Animation> GetAnimationStates(Vector2i clip_size, Vector2i sheet_dimensions) = 0;

	Sprite* owner;
};