/**
	Panel de Pon (Tetris Attack) clone
	Made from scratch as a C++ programming exercise

	NOTE: audio assets borrowed from tetris attack (waiting on audio guy), all other assets are homemade

	@author Conor MacKeigan 2017
*/

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_ttf.lib")
#pragma comment(lib, "SDL2_mixer.lib")

extern "C" 
{
	#include <SDL.h>
	#include <SDL_image.h>
}

#include <iostream>
#include <sstream>
#include <chrono>

#include "Settings.h"
#include "Game.h"
#include "Input.h"

#include "GameManager.h"

#ifndef NDEBUG
#	include "UIText.h"
#	include "UIManager.h"
#endif

#define FPS 60.0 // Fixed timestep
#define input Input::Instance()

int main(int argc, char *argv[])
{
	// Init SDL and window
	SDL_Window* window = nullptr;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO) < 0)
		std::cerr << "Error initializing SDL system: " << SDL_GetError() << std::endl;

	window = SDL_CreateWindow("ATTACK ON PANEL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	// Init input
	input.Init();

	// Create game
	Game game;
	game.Init(window);

	// Game loop
	SDL_Event e;

	// Fix timestep to 60fps
	const double millis_per_frame = (1000.0 / FPS); // milliseconds per frame
	Uint64 now = SDL_GetPerformanceCounter();
	Uint64 last;
	double deltaTime = 0.0;

#ifndef NDEBUG
	// Turbo framerate (fast as possible - hold TAB)
	bool turbo = false;

	// Debug variables
	auto chrono_last = std::chrono::high_resolution_clock::now();

	UIText* fps_text = UIManager::Instance().Factory<UIText>();
	fps_text->SetDrawOrder(2147483647);
	fps_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
	fps_text->color = { 0xFF, 0xF3, 0x05, 0xFF };
#endif

	while (!GameManager::Instance().quit_app)
	{
#ifndef NDEBUG
		if (input.GetKey(SDL_SCANCODE_TAB))
		{
			turbo = true;
			now = SDL_GetPerformanceCounter();
		}
		else
		{
			turbo = false;
		}

		if (!turbo)
		{
#endif
			// Wait if faster than 60fps
			while (deltaTime < millis_per_frame)
			{
				last = now;
				now = SDL_GetPerformanceCounter();

				deltaTime += (double)((now - last) * 1000.0 / SDL_GetPerformanceFrequency());
			}

			deltaTime -= millis_per_frame;

#ifndef NDEBUG
		} // Close turbo check

		// Debug framerate TODO: update text once per second instead of every frame
		auto chrono_now = std::chrono::high_resolution_clock::now();
		auto difference = (chrono_now - chrono_last).count();
		chrono_last = chrono_now;

		double fps = 1000000000.0 / difference;
#endif

		// Event loop
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				// Keyboard input
			case SDL_KEYDOWN:
				input.RegKeyDown(e.key.keysym.scancode, e.key.repeat > 0);
				break;
			case SDL_KEYUP:
				input.RegKeyUp(e.key.keysym.scancode, e.key.repeat > 0);
				break;

				// Mouse input
			case SDL_MOUSEMOTION:
				input.mouse_position = Vector2i(e.motion.x, e.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				input.RegMouseButtonDown(e.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				input.RegMouseButtonUp(e.button.button);
				break;
			case SDL_MOUSEWHEEL:
				// TODO
				break;

				// Controller input
			case SDL_CONTROLLERBUTTONDOWN:
				input.RegControllerButtonDown(e.cbutton.button);
				break;
			case SDL_CONTROLLERBUTTONUP:
				input.RegControllerButtonUp(e.cbutton.button);
				break;

				// Controller connect/disconnect
			case SDL_CONTROLLERDEVICEADDED:
				input.AddController(e.cdevice.which);
				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				input.RemoveController(e.cdevice.which);
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					game.Quit();
					break;

				case SDL_WINDOWEVENT_FOCUS_GAINED:
					now = SDL_GetPerformanceCounter();
					break;

				case SDL_WINDOWEVENT_MOVED:
					now = SDL_GetPerformanceCounter();
					break;
				}

				break;

				// Application quit condition
			case SDL_QUIT:
				game.Quit();
				break;
			}
		}

#ifndef NDEBUG
		// Frame advance
		if (input.GetKey(SDL_SCANCODE_BACKSLASH) && !input.GetKeyPressed(SDL_SCANCODE_RIGHTBRACKET)) { continue; }
#endif
		// Update game systems
		game.Update(window);

		input.FlushEvents();

		game.Render();

#ifndef NDEBUG
		// Render FPS in top left of screen
		std::stringstream ss;
		ss << (int)fps << " FPS";
		fps_text->text = ss.str();
#endif

	}

	// Cleanup
#ifndef NDEBUG
	UIManager::Instance().DestroyUIElement(fps_text);
#endif

	game.Shutdown();

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}