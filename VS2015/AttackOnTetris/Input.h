#pragma once
#pragma warning(disable:4800) // int to bool conversion warning

extern "C" {
#include <SDL.h>
#include "SDL_gamecontroller.h"
}

#include <vector>
#include "Vector2.h"


/**
	Purpose: Track all input states from central event poll.	 
	@author Conor MacKeigan 2017
*/
class Input
{
	// The function that polls events needs access to the registration methods
	friend int ::main(int argc, char* argv[]);

private:
	Input() = default;
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

public:
	static Input& Instance();
	~Input() = default;

	void Init();

private:
	// Called at the end of every frame to clear press/release states
	void FlushEvents();

	//----------------------------------------------------------------------
	//	Keyboard
	//----------------------------------------------------------------------
public:
	// Public state getters
	bool GetKey(SDL_Scancode key); // is the key currently down?
	bool GetKeyPressed(SDL_Scancode key); // was the key pressed this frame?
	bool GetKeyUp(SDL_Scancode key); // was the key released this frame?

private:
	// Key states
	bool keys[SDL_NUM_SCANCODES];
	bool keys_down[SDL_NUM_SCANCODES];
	bool keys_up[SDL_NUM_SCANCODES];

	// Polling
	void RegKeyDown(SDL_Scancode key, bool key_repeat);
	void RegKeyUp(SDL_Scancode key, bool key_repeat);

	// Log key events each frame for efficient clearing at beginning of frame
	std::vector<SDL_Scancode> key_down_clears;
	std::vector<SDL_Scancode> key_up_clears;

	//----------------------------------------------------------------------
	//	Mouse		
	//	TODO: scroll delta					
	//----------------------------------------------------------------------
public:
	enum MouseButton
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT = 1,
		MOUSE_MIDDLE = 2,
		MOUSE_X1 = 3,
		MOUSE_X2 = 4
	};

	// public states
	Vector2f MousePosition();
	
	bool GetMouseButton(MouseButton button);
	bool GetMouseButtonPressed(MouseButton button);
	bool GetMouseButtonReleased(MouseButton button);
	
private:
	// Mouse states
	Vector2f mouse_position;
	Uint8 mouse_buttons;
	Uint8 mouse_buttons_pressed;
	Uint8 mouse_buttons_released;

	// Polling
	void RegMouseButtonDown(Uint8 button);
	void RegMouseButtonUp(Uint8 button);

	//----------------------------------------------------------------------
	//	Joystick				
	//----------------------------------------------------------------------
private:
	SDL_GameController* controllers[8];

public:
	// Public state getters
	bool GetControllerButton(SDL_GameControllerButton button);
	bool GetControllerButtonPressed(SDL_GameControllerButton button);
	bool GetControllerButtonReleased(SDL_GameControllerButton button);

private:
	// Controller button states (TODO: axes)
	Uint16 controller_buttons;
	Uint16 controller_buttons_pressed;
	Uint16 controller_buttons_released;

	// Polling
	void RegControllerButtonDown(Uint8 button);
	void RegControllerButtonUp(Uint8 button);

	// Update opened controllers on connect/disconnect
	void AddController(Sint32 index);
	void RemoveController(Sint32 index);
};

