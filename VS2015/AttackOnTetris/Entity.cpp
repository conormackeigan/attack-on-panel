#include "Entity.h"
#include <memory>

void Entity::Init() { }
void Entity::Update() { }
void Entity::OnDestroy() { }

bool Entity::IsActive()
{
	return active;
}

void Entity::SetActive(bool active)
{
	this->active = active;
}

bool Entity::IsSprite()
{
	return false;
}

bool Entity::IsPooled()
{
	return false;
}

