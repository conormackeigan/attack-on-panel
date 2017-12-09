#pragma once

#include "Sprite.h"

/**
	Purpose: Pooled digit sprites that are only rendered and not updated

	@author Conor MacKeigan 2017
*/
class Digit : public Sprite
{
public:
	Digit(TextureManager::TEXTURE_NAME texture_name);

	unsigned int digit; // What digit is this?
	unsigned int index; // Index of unique_ptr in object pool
};

