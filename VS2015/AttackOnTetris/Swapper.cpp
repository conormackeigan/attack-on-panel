#include "Swapper.h"
#include "AnimatorSwapper.h"
#include "TextureManager.h"

Swapper::Swapper() : Sprite(TextureManager::SWAPPER)
{
	animator = std::make_unique<AnimatorSwapper>(this);
	drawOrder = 1000;
}

Swapper::~Swapper() { }

void Swapper::Init() { }

void Swapper::Update() { }