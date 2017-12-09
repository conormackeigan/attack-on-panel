/**
	Purpose: Track all input states from central event poll

	@author Conor MacKeigan 2017
*/

#include "Input.h"
#include "StandardTypes.h"


Input& Input::Instance()
{
	static Input instance;

	return instance;
}

void Input::Init()
{
	for (int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		keys[i] = false;
		keys_down[i] = false;
		keys_up[i] = false;
	}

	key_down_clears.reserve(16);
	key_up_clears.reserve(16);

	mouse_position = Vector2f::zero;

	// Init game controller(s)
	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");

	for (int i = 0; i < 8; i++)
		controllers[i] = nullptr;

	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
			SDL_GameControllerOpen(i);
	}
}

void Input::FlushEvents()
{
	// Clear key press/release events
	while (!key_down_clears.empty())
	{
		keys_down[key_down_clears.back()] = false;
		key_down_clears.pop_back();
	}

	while (!key_up_clears.empty())
	{
		keys_up[key_up_clears.back()] = false;
		key_up_clears.pop_back();
	}

	// Clear mouse press/release events
	mouse_buttons_pressed = Flag::none;
	mouse_buttons_released = Flag::none;

	// Clear joystick press/release events 
	controller_buttons_pressed = Flag::none;
	controller_buttons_released = Flag::none;
}

bool Input::GetKey(SDL_Scancode key)
{
	return keys[key];
}

bool Input::GetKeyPressed(SDL_Scancode key)
{
	return keys_down[key];
}

bool Input::GetKeyUp(SDL_Scancode key)
{
	return keys_up[key];
}

void Input::RegKeyDown(SDL_Scancode key, bool key_repeat)
{
	keys[key] = true;

	if (!key_repeat)
	{
		keys_down[key] = true;
		key_down_clears.push_back(key);
	}
}

void Input::RegKeyUp(SDL_Scancode key, bool key_repeat)
{
	keys[key] = false;

	if (!key_repeat)
	{
		keys_up[key] = true;
		key_up_clears.push_back(key);
	}
}

Vector2f Input::MousePosition()
{
	return Vector2f(mouse_position);
}

bool Input::GetMouseButton(MouseButton button)
{
	return mouse_buttons & 1 << button;
}

bool Input::GetMouseButtonPressed(MouseButton button)
{
	return mouse_buttons_pressed & 1 << button;
}

bool Input::GetMouseButtonReleased(MouseButton button)
{
	return mouse_buttons_released & 1 << button;
}

void Input::RegMouseButtonDown(Uint8 button)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		mouse_buttons |= flag1;
		mouse_buttons_pressed |= flag1;
		break;
	case SDL_BUTTON_RIGHT:
		mouse_buttons |= flag2;
		mouse_buttons_pressed |= flag2;
		break;
	case SDL_BUTTON_MIDDLE:
		mouse_buttons |= flag3;
		mouse_buttons_pressed |= flag3;
		break;
	case SDL_BUTTON_X1:
		mouse_buttons |= flag4;
		mouse_buttons_pressed |= flag4;
		break;
	case SDL_BUTTON_X2:
		mouse_buttons |= flag5;
		mouse_buttons_pressed |= flag5;
		break;
	}
}

void Input::RegMouseButtonUp(Uint8 button)
{
	switch (button)
	{
	case SDL_BUTTON_LEFT:
		mouse_buttons &= ~flag1;
		mouse_buttons_released |= flag1;
		break;
	case SDL_BUTTON_RIGHT:
		mouse_buttons &= ~flag2;
		mouse_buttons_released |= flag2;
		break;
	case SDL_BUTTON_MIDDLE:
		mouse_buttons &= ~flag3;
		mouse_buttons_released |= flag3;
		break;
	case SDL_BUTTON_X1:
		mouse_buttons &= ~flag4;
		mouse_buttons_released |= flag4;
		break;
	case SDL_BUTTON_X2:
		mouse_buttons &= ~flag5;
		mouse_buttons_released |= flag5;
		break;
	}
}

bool Input::GetControllerButton(SDL_GameControllerButton button)
{
	return controller_buttons & 1 << button;
}

bool Input::GetControllerButtonPressed(SDL_GameControllerButton button)
{
	return controller_buttons_pressed & 1 << button;
}

bool Input::GetControllerButtonReleased(SDL_GameControllerButton button)
{
	return controller_buttons_released & 1 << button;
}

void Input::RegControllerButtonDown(Uint8 button)
{
	controller_buttons |= 1 << button;
	controller_buttons_pressed |= 1 << button;
}

void Input::RegControllerButtonUp(Uint8 button)
{
	controller_buttons &= ~(1 << button);
	controller_buttons_released |= 1 << button;
}

void Input::AddController(Sint32 index)
{
	if (index > 7)
	{
		std::cerr << "Somehow more than 8 controllers have been connected...";
		return;
	}

	controllers[index] = SDL_GameControllerOpen(index);
}

void Input::RemoveController(Sint32 index)
{
	if (index > 7)
	{
		std::cerr << "Somehow trying to disconnect a controller with an index greater than the limit of 8 controllers. Wuh";
		return;
	}

	SDL_GameControllerClose(controllers[index]);
	controllers[index] = nullptr;
}