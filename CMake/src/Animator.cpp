#include "Animator.h"
#include "Sprite.h"
#include "StandardTypes.h"

Animator::Animator(Sprite* owner, TextureManager::TEXTURE_NAME texture_name, Vector2i clip_size, Vector2i sheet_dimensions, std::unordered_map<std::string, Animation> animation_states, std::string default_state) : 
	owner(owner), texture_name(texture_name), clip_size(clip_size), sheet_dimensions(sheet_dimensions), animation_states(animation_states), default_state(default_state)
{
	SetState(default_state);
}

// tick animator and update frame
void Animator::Update()
{
	if (ticks_per_frame < 1 || curr_animation == nullptr)
	{
		// don't animate sequences with 0/negative speed, or single-clip states
		return;
	}

	// Change frames
	if (++ticks >= ticks_per_frame)
	{
		ticks = 0;

		// Set frame to next in animation, wrapping to start if necessary
		if (++curr_frame >= curr_animation->srcRects.size())
		{
			// Wrap if loop is set
			if (curr_animation->loop)
			{
				curr_frame = 0;
			}
			else
			{
				curr_animation = nullptr;
				return;
			}
		}

		// Update sprite's active frame
		owner->src = curr_animation->srcRects[curr_frame];
	}
}

void Animator::SetState(std::string state)
{
	// Reset driving variables
	ticks = 0;
	curr_frame = 0;

	std::unordered_map<std::string, Animation>::iterator it = animation_states.find(state);

	// Couldn't find animation
	ASSERT(it != animation_states.end(), "Animation state '" << state << "' not found. Check Animator." << std::endl);

	// Set current animation to the new state
	curr_animation = &(it->second);
	owner->src = curr_animation->srcRects[0];

	// Calculate state animation speed
	if (curr_animation->srcRects.size() < 2)
	{
		// If animation state contains only a single frame (which has already been set), it's no longer needed
		curr_animation = nullptr;
	}
	else if (curr_animation->speed <= 0)
	{
		// Animation is not driven, but the speed parameter might change so keep a reference to the animation
		ticks_per_frame = -1;
	}
	else // don't divide by 0 pls
	{
		ticks_per_frame = 1.f / curr_animation->speed;
	}
}

