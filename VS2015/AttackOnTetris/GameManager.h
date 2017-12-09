#pragma once

#include "Settings.h"
#include "Vector2.h"
#include "GameGrid.h"
#include <memory>

class ScreenFade;
class MainMenu;

// Note: all the static methods are just fudging callbacks to be simpler (it's a singleton anyway)
class GameManager
{
public:
	static GameManager& Instance();

private:
	GameManager() = default;

public:
	~GameManager() = default;

	GameManager(const GameManager&) = delete;
	void operator=(const GameManager&) = delete;

	void Init();

	// for main game loop
	bool quit_app = false;

	void QuitApp();

	// Called when a transition is complete
	typedef void(*transition_callback_function)(void);

	static void StartGameSession(GameMode mode, Difficulty difficulty, Vector2i grid_size);
	static void CloseMenuLoadGrid();
	static void StartGrid();

	static void ResumeGame();
	static void QuitGame();
	static void ReturnToMainMenu();
	static void UnlockMainMenu();

	// Game session parameters
	GameMode game_mode;
	Difficulty difficulty;
	Vector2i grid_size;

	// Game objects
	MainMenu* main_menu;
	GameGrid* game_grid;

	
};

