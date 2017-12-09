#pragma once

#include "Vector2.h"

/**
	Purpose: Abstraction layer between input states and game actions

	@author Conor MacKeigan 2017
*/
class InputController
{
private:
	InputController() = default;

public:
	static InputController& Instance();
	~InputController() = default;

	void Update(); // For directional press-repeat on hold

	bool PressUp();
	bool PressDown();
	bool PressRight();
	bool PressLeft();

	bool HoldUp();
	bool HoldDown();
	bool HoldRight();
	bool HoldLeft();

	const unsigned int hold_to_press_time = 12; // Number of frames a directional input is held before repeat-press
	unsigned int hold_up_time = 0;
	unsigned int hold_down_time = 0;
	unsigned int hold_right_time = 0;
	unsigned int hold_left_time = 0;

	bool PressStart();
	bool PressAction();
	bool PressCancel();
	bool PressLeftShoulderButton();
	bool PressRightShoulderButton();
	bool HoldLeftShoulder();
	bool HoldRightShoulder();

	bool MouseLeftClick();
	Vector2f MousePosition();
};
