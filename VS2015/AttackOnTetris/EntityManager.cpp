#include "EntityManager.h"
#include "RenderSystem.h"
#include "IPooled.h"
#include "Animator.h"
#include "StandardTypes.h"

// Entity includes
#include "MainMenu.h"
#include "Swapper.h"


#define render_system RenderSystem::Instance()

// Singleton
EntityManager& EntityManager::Instance()
{
	static EntityManager instance;

	return instance;
}

void EntityManager::Init()
{
	// Reserve memory for entities
	entities.reserve(256);
	entities_to_delete.reserve(256);
	entities_to_recycle.reserve(256);
}

void EntityManager::Update()
{
	// Call Update() on all entities
	for (EntityColony::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if ((*it)->IsActive())
			(*it)->Update();
	}

	DeleteDestroyedEntities();
	RemoveRecycledEntities();
}

void EntityManager::DestroyEntity(Entity* entity)
{
	entities_to_delete.push_back(entity);
}

void EntityManager::DeleteDestroyedEntities()
{
	for (std::vector<Entity*>::iterator it = entities_to_delete.begin(); it != entities_to_delete.end(); ++it)
	{
		(*it)->OnDestroy();

		// If entity is a sprite it needs to be unregistered from the RenderSystem
		if ((*it)->IsSprite())
		{
			Sprite* sprite = static_cast<Sprite*>(*it);
			render_system.UnregisterRenderable(sprite);

			if (sprite->animator != nullptr)
				render_system.UnregisterAnimator(sprite->animator.get());
		}

		EntityColony::iterator iter;
		for (iter = entities.begin(); iter != entities.end(); ++iter)
		{
			if (*iter == *it)
				break;
		}

		ASSERT(iter != entities.end(), "Trying to delete an entity not registered in EntityManager");

		// Don't delete entities from an Object Pool TODO: this is only for panel's hardcoded pool, implement IPooled
		if (!(*iter)->IsPooled())
			delete *iter;

		entities.erase(iter);
	}

	entities_to_delete.clear();
}

void EntityManager::RecycleEntity(Entity* entity)
{
	entities_to_recycle.push_back(entity);
}

void EntityManager::RemoveRecycledEntities()
{
	for (std::vector<Entity*>::iterator it = entities_to_recycle.begin(); it != entities_to_recycle.end(); ++it)
	{
		(*it)->OnDestroy();

		// If entity is a sprite it needs to be unregistered from the RenderSystem
		if ((*it)->IsSprite())
		{
			Sprite* sprite = static_cast<Sprite*>(*it);
			render_system.UnregisterRenderable(sprite);

			if (sprite->animator != nullptr)
				render_system.UnregisterAnimator(sprite->animator.get());
		}

		EntityColony::iterator iter;
		for (iter = entities.begin(); iter != entities.end(); ++iter)
		{
			if (*iter == *it)
				break;
		}

		ASSERT(iter != entities.end(), "Trying to delete an entity not registered in EntityManager");

		entities.erase(iter);
	}

	entities_to_recycle.clear();
}

void EntityManager::DestroyAllEntities()
{
	for (EntityColony::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		entities_to_delete.push_back(*it);
	}
}

void EntityManager::RegisterEntity(Entity* entity)
{
	entities.insert(entity);

	if (entity->IsSprite())
	{
		Sprite* sprite = static_cast<Sprite*>(entity);

		render_system.RegisterRenderable(sprite);
		render_system.RegisterAnimator(sprite->animator.get());
	}
}

//void EntityManager::UnregisterExternalEntity(Entity* entity)
//{
//	// If entity is a sprite it needs to be unregistered from the RenderSystem
//	if (entity->IsSprite())
//	{
//		Sprite* sprite = static_cast<Sprite*>(entity);
//		render_system.UnregisterRenderable(sprite);
//		render_system.UnregisterAnimator(sprite->animator.get());
//	}
//
//	EntityColony::iterator it;
//	for (it = entities.begin(); it != entities.end(); ++it)
//	{
//		if (*it == entity)
//			break;
//	}
//}
