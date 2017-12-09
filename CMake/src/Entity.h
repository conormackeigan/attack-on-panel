#pragma once

/**
	Purpose: Base class of all game agents

	@author Conor MacKeigan 2017
*/
class Entity
{
public:
	virtual ~Entity() = default;

protected:
	// Inactive entities are not updated
	bool active = true; 

public:
	virtual void Init();
	virtual void Update();

	// Pass ownership reference in case of pool recycling
	virtual void OnDestroy();
	
	bool IsActive();
	virtual void SetActive(bool active);

	// RTTI lul
	virtual bool IsSprite();
	virtual bool IsPooled();
};