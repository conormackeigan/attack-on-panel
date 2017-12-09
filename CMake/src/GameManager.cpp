#include "GameManager.h"
#include "EntityManager.h"
#include "UIManager.h"
#include "ScreenFader.h"
#include "MainMenu.h"
#include "PauseMenu.h"


#define entity_manager EntityManager::Instance()
#define screen_fader   ScreenFader::Instance()
#define game_manager   GameManager::Instance()

GameManager& GameManager::Instance()
{
	static GameManager instance;

	return instance;
}

void GameManager::Init()
{
	// Start with main Menu
	main_menu = entity_manager.Factory<MainMenu>();
}

void GameManager::QuitApp()
{
	quit_app = true;
}

// Transition callback after main menu sequence
void GameManager::StartGameSession(GameMode game_mode, Difficulty difficulty, Vector2i grid_size)
{
	game_manager.game_mode = game_mode;
	game_manager.difficulty = difficulty;
	game_manager.grid_size = grid_size;

	// Fade menu out
	screen_fader.FadeOut(&CloseMenuLoadGrid);
}

// Transition callback after main menu fadeout. Initializes the game grid.
void GameManager::CloseMenuLoadGrid()
{
	game_manager.main_menu->SetActive(false);

	game_manager.game_grid = entity_manager.Factory<GameGrid>(game_manager.game_mode, game_manager.grid_size, game_manager.difficulty);
	screen_fader.FadeIn(&StartGrid);
}

void GameManager::StartGrid()
{
	// Start the panel fadein and countdown
	game_manager.game_grid->StartGame();
}

void GameManager::ResumeGame()
{
	game_manager.game_grid->unpausing = true;
}

void GameManager::QuitGame()
{
	screen_fader.FadeOut(&ReturnToMainMenu);
}

void GameManager::ReturnToMainMenu()
{
	entity_manager.DestroyEntity(game_manager.game_grid);
	screen_fader.FadeIn(&UnlockMainMenu);
	game_manager.main_menu->SetActive(true);
}

void GameManager::UnlockMainMenu()
{
	game_manager.main_menu->state = MainMenu::SELECT_DIFFICULTY;
}
