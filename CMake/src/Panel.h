#pragma once

extern "C" {
#include <SDL.h>
}

#include "IPooled.h"
#include <unordered_map>
#include <vector>

/**
	Purpose: Colored panels. Will need to refactor for garbage and ! blocks if they are ever to be a thing

	@author Conor MacKeigan 2017
*/

#define POOL_SIZE 1024

class Panel : public IPooled<Panel>
{
public:
	enum PanelType
	{
		Empty,
		Blank,
		Red,
		Green,		
		Yellow,
		Cyan,
		Purple,
		Blue,

		NUM_PANEL_TYPES
	};	

	enum State
	{
		Idle, // swappable
		Swapping, // re-swappable
		Hovering, // not swappable
		Falling, // swappable
		Flashing, // not swappable
		Clearing // not swappable
	};

	// Constants
	static const uint8_t can_swap_mask;
	static const uint8_t can_fall_mask;
	static const uint8_t can_clear_mask;
	static const uint8_t can_chain_mask;
	
	// Pixel size of a basic panel
	static const int SIZE;

public:
	Panel();
	~Panel();

	// All panels default to blank until SetPanelType() is called
	PanelType panel_type = Blank;

	std::unique_ptr<Sprite> dead_sprite = nullptr;
	bool dead = true;

	State state = Idle; // Current state
	State prev_state; // Preserve old state during swap

	Vector2i grid_pos;

	unsigned int hover_time;
	bool can_chain = false;

	static void Swap(Panel& lhs, Panel& rhs);

	void Init() override;
	void Update() override;
	void OnDestroy() override;

	void Render(SDL_Renderer* renderer) override;

	void SetState(State s);

	void Die();

	bool CanSwap();
	bool CanFall();
	bool CanClear();
	bool CanChainFall();

	void Pop();

	// Object pool "overloaded override"
	static Panel* GetFromPool(PanelType type);

	// DEBUG
	std::string ToString();

private:
	void SetPanelType(PanelType panel_type); // Only to be called when initializing pool
};

bool operator<(const Panel& lhs, const Panel& rhs);