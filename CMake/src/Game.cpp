#include "Game.h"
#include "Settings.h"
#include "GameManager.h"
#include "RenderSystem.h"
#include "EntityManager.h"
#include "UIManager.h"
#include "AudioManager.h"
#include "UIText.h"
#include "MainMenu.h"
#include "Input.h"
#include "InputController.h"
#include <time.h>
#include <iostream>
#include "LogSpiralParticle.h"
#include "DigitPool.h"
#include "ComboSign.h"
#include "ChainSign.h"

#define game_manager	 GameManager::Instance()
#define render_system	 RenderSystem::Instance()
#define entity_manager	 EntityManager::Instance()
#define ui_manager		 UIManager::Instance()
#define audio_manager	 AudioManager::Instance()
#define input_controller InputController::Instance()

void Game::Init(SDL_Window* window)
{
	// Random seeding
	srand(time(nullptr));

	render_system.Init(window);
	entity_manager.Init();
	ui_manager.Init();

	// Initialize asset managers (set filepath mappings)
	textureManager.Init();
	fontManager.Init();

	// Load all textures & fonts cuz this game is tiny
	textureManager.Load();
	fontManager.Load();

	// Load all sounds
	audio_manager.Init();
	
	// Initialize object pools
	Panel::InitPool(1024);
	LogSpiralParticle::InitPool(128);
	DigitPool::InitPool(48);
	ComboSign::InitPool(128);
	ChainSign::InitPool(128);

	// Roll the ball
	game_manager.Init();

	// test
	/*LogSpiralParticle* test1 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 0.f);
	LogSpiralParticle* test2 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 60.f);
	LogSpiralParticle* test3 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 120.f);
	LogSpiralParticle* test4 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 180.f);
	LogSpiralParticle* test5 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 240.f);
	LogSpiralParticle* test6 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 300.f);*/

	/*LogSpiralParticle* test7 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 30.f);
	LogSpiralParticle* test8 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 90.f);
	LogSpiralParticle* test9 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 150.f);
	LogSpiralParticle* test10 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 210.f);
	LogSpiralParticle* test11 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 270.f);
	LogSpiralParticle* test12 = entity_manager.Factory<LogSpiralParticle>(Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT) / 2.f, 330.f);*/
}

void Game::Update(SDL_Window* window)
{
	// Temp
	if (Input::Instance().GetKeyPressed(SDL_SCANCODE_M))
		audio_manager.ToggleMute();

	if (Input::Instance().GetKeyPressed(SDL_SCANCODE_Q))
		std::cout << "Renderables: " << render_system.renderables.size() << std::endl << "Animators: " << render_system.animators.size() << std::endl;

	input_controller.Update(); // Abstraction layer for determining input-repeat on hold

	// Update core systems
	entity_manager.Update();
}

void Game::Render()
{
	render_system.Render();
}

void Game::Shutdown()
{
	// Release renderer
	render_system.Shutdown();

	// Release assets
	textureManager.Shutdown();
	fontManager.Shutdown();
}

void Game::Quit()
{
	game_manager.QuitApp();
}
