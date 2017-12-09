#pragma once

#include "Entity.h"
#include "Sprite.h"
#include "RenderSystem.h"
#include <vector>
#include "Vector2.h"
#include "plf_colony.h"

class Animator;

template <typename T>
class IPooled;

/**
	Purpose: Handles ownership of all game entities, and their game logic every Update()

	@author Conor MacKeigan 2017
*/

typedef plf::colony<Entity*> EntityColony;

class EntityManager
{
public:
	static EntityManager& Instance();

	EntityManager(const EntityManager&)  = delete;
	void operator=(const EntityManager&) = delete;

	void Init();

	// Call Update() on all Entities
	void Update();

	//-----------------------------------------------------
	//	Factory
	//-----------------------------------------------------
	template<typename T, typename... Args>
	T* Factory(Args... args)
	{
		EntityColony::iterator it = entities.insert(new T(args...));
		Entity* entity = *it;

		assert(entity->IsPooled() == false);

		// If this entity is a sprite and active, tell the RenderSystem to render it every frame
		if (entity->IsSprite())
		{
			Sprite* sprite = static_cast<Sprite*>(entity);

			RenderSystem::Instance().RegisterRenderable(static_cast<IRenderable*>(sprite));
			RenderSystem::Instance().RegisterAnimator(sprite->animator.get());
		}

		entity->Init();

		return static_cast<T*>(entity);
	}

	// Unfactory
	std::vector<Entity*> entities_to_delete;
	std::vector<Entity*> entities_to_recycle;

	void DestroyEntity(Entity* entity);
	void DestroyAllEntities();
	void DeleteDestroyedEntities();

	// "Recycle" pooled objects (unregister but do not delete)
	void RecycleEntity(Entity* entity);
	void RemoveRecycledEntities();

	// External registration
	void RegisterEntity(Entity* entity);

private:
	EntityManager() = default;

	// All entities active in the game
	EntityColony entities;
};

