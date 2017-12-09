#pragma once

#include "Sprite.h"
#include "EntityManager.h"
#include <vector>

#define entity_manager EntityManager::Instance()

/**
	Interface to open and distribute objects from a pool

	@author Conor MacKeigan 2017
*/
template <typename T>
class IPooled : public Sprite
{
public:
	IPooled<T>() : Sprite() { }
	IPooled<T>(TextureManager::TEXTURE_NAME texture_name) : Sprite(texture_name) { }

	static std::vector<std::unique_ptr<T>>& pool() { static std::vector<std::unique_ptr<T>> pool; return pool; }
	static std::vector<unsigned int>& next_available() { static std::vector<unsigned int> next_available; return next_available; }

	unsigned int index;

	static void InitPool(unsigned int pool_size)
	{
		pool().reserve(pool_size);
		next_available().reserve(pool_size);

		for (int i = 0; i < pool_size; i++)
		{
			pool().push_back(std::make_unique<T>());

			pool().back()->index = i;
			next_available().push_back(i);
		}
	}

	static T* GetFromPool()
	{
		T* item = pool()[next_available().back()].get();
		next_available().pop_back();
		item->Reset();
		entity_manager.RegisterEntity(item);

		return item;
	}

	virtual void OnDestroy() override
	{
		next_available().push_back(index);
	}

	virtual void Reset() { }

	virtual void Recycle()
	{
		entity_manager.RecycleEntity(this);
	}
};
