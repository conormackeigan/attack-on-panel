#pragma once

extern "C" {
#include <SDL.h>
}

#include "Entity.h"
#include "Vector2.h"
#include "Panel.h"
#include "Settings.h"

class UIText;

#define MIN_GRID_WIDTH 6
#define MIN_GRID_HEIGHT 12
#define MAX_GRID_WIDTH 16
#define MAX_GRID_HEIGHT 14

// Pixel height of a line of text
#define TEXT_HEIGHT 48

#define MIN_SCROLL_SPEED 0.2f

/**
	Purpose: The first scene in the game. Sets up a game session.
	Here be spaghetti, never forgetti

	@author Conor MacKeigan 2017
*/
class MainMenu : public Entity
{
public:
	enum State
	{
		SELECT_MODE,
		SELECT_SIZE,
		SELECT_DIFFICULTY,
		CLOSING
	};

	MainMenu() = default;
	~MainMenu() = default;

	void Init();
	void Update();
	void OnDestroy();

	void SetActive(bool active) override;

	State state = SELECT_MODE;

private:
	void PopulatePanelGrid();
	void TransitionState(State new_state);
	void UpdatePanelPositions();

	State next_state; // For transitions

	// How far left of the menu items the pointer is placed
	const Vector2f POINTER_MARGIN = { 32.f, 0.f };

	// Game session parameters
	GameMode game_mode;
	Vector2i grid_size;
	Difficulty difficulty;	

	// Menu items
	void UpdateGridSizeText();

	UIText* text_mode_select[3];

	UIText* text_grid_size;
	Panel* panel_grid[MAX_GRID_WIDTH][MAX_GRID_HEIGHT];
	Panel::PanelType grid_types[MAX_GRID_WIDTH][MAX_GRID_HEIGHT];

	UIText* text_difficulty_select[3];

	// Positions of menu items
	Vector2f text_mode_select_position[3];

	Vector2f text_grid_size_position;
	Vector2f panel_grid_position;

	Vector2f text_difficulty_select_position[3];

	// A '>' character that points at the current menu selection
	UIText* pointer_mode_select;
	UIText* pointer_difficulty_select;

	// Index of menu item '>' is pointing at
	int selected_mode_select = 0;
	int selected_difficulty_select = 0;

	// How far from origin the menu has been scrolled 
	bool scrolling = false;

	Vector2f scroll_offset;
	float desired_scroll_offset = 0; // For lerping screen scroll
	const float state_scroll_offset[3] = { 0, SCREEN_HEIGHT, SCREEN_HEIGHT * 2 };
};

