#include "AnimatorPanel.h"


AnimatorPanel::AnimatorPanel(Sprite* owner) : Animator(owner, TextureManager::PANEL_RED, Vector2i(32, 32), Vector2i(3, 1), GetAnimationStates(Vector2i(32, 32), Vector2i(3, 1)), "Idle")
{
}


AnimatorPanel::~AnimatorPanel()
{
}

std::unordered_map<std::string, Animation> AnimatorPanel::GetAnimationStates(Vector2i clip_size, Vector2i sheet_dimensions)
{
	return
	{ 
		{ "Idle",		Animation({ 0 },	sheet_dimensions, clip_size) },
		{ "Flashing",	Animation({ 0, 1 },	sheet_dimensions, clip_size) },
		{ "Clearing",	Animation({ 2 },	sheet_dimensions, clip_size) } 
	};
}
