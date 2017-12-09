#include "InputController.h"
#include "Input.h"
#include <iostream>

#define input Input::Instance()

// TODO: refactor from singleton if multiple players is ever to be a thing
InputController& InputController::Instance()
{
	static InputController instance;

	return instance;
}

void InputController::Update()
{
	if (HoldUp())
		++hold_up_time;
	else
		hold_up_time = 0;

	if (HoldDown())
		++hold_down_time;
	else
		hold_down_time = 0;

	if (HoldRight())
		++hold_right_time;
	else
		hold_right_time = 0;

	if (HoldLeft())
		++hold_left_time;
	else
		hold_left_time = 0;
}

bool InputController::PressUp()
{
	return hold_up_time > hold_to_press_time || input.GetKeyPressed(SDL_SCANCODE_UP) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
}

bool InputController::PressDown()
{
	return hold_down_time > hold_to_press_time || input.GetKeyPressed(SDL_SCANCODE_DOWN) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
}

bool InputController::PressRight()
{
	return hold_right_time > hold_to_press_time || input.GetKeyPressed(SDL_SCANCODE_RIGHT) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
}

bool InputController::PressLeft()
{
	return hold_left_time > hold_to_press_time || input.GetKeyPressed(SDL_SCANCODE_LEFT) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
}

bool InputController::HoldUp()
{
	return input.GetKey(SDL_SCANCODE_UP) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
}

bool InputController::HoldDown()
{
	return input.GetKey(SDL_SCANCODE_DOWN) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
}

bool InputController::HoldLeft()
{
	return input.GetKey(SDL_SCANCODE_LEFT) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
}

bool InputController::HoldRight()
{
	return input.GetKey(SDL_SCANCODE_RIGHT) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
}

bool InputController::PressStart()
{
	return input.GetKeyPressed(SDL_SCANCODE_RETURN) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
}

bool InputController::PressAction()
{
	return input.GetKeyPressed(SDL_SCANCODE_SPACE) || input.GetKeyPressed(SDL_SCANCODE_Z) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
}

bool InputController::PressCancel()
{
	return input.GetKeyPressed(SDL_SCANCODE_X) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
}

bool InputController::PressLeftShoulderButton()
{
	return input.GetKeyPressed(SDL_SCANCODE_A) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
}

bool InputController::PressRightShoulderButton()
{
	return input.GetKeyPressed(SDL_SCANCODE_S) || input.GetControllerButtonPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
}

bool InputController::HoldLeftShoulder()
{
	return input.GetKey(SDL_SCANCODE_A) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
}

bool InputController::HoldRightShoulder()
{
	return input.GetKey(SDL_SCANCODE_S) || input.GetControllerButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
}

bool InputController::MouseLeftClick()
{
	return input.GetMouseButtonPressed(Input::MouseButton::MOUSE_LEFT);
}

Vector2f InputController::MousePosition()
{
	return input.MousePosition();
}