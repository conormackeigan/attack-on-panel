#pragma once

#include "GameManager.h"

class ScreenFade;

/**
	Purpose: Simple fade in/out screen transitions
	Remarks: A callback function must be provided to be called when the fade is complete

	@author Conor MacKeigan 2017
*/
class ScreenFader
{
public:
	enum Fade
	{
		FADE_IN,
		FADE_OUT
	};

	static ScreenFader& Instance();
	~ScreenFader() = default;

	ScreenFader(const ScreenFader&) = delete;
	void operator=(const ScreenFader&) = delete;

	void FadeIn(GameManager::transition_callback_function callback);
	void FadeOut(GameManager::transition_callback_function callback);
	void FinishFade();

private:
	ScreenFader() = default;

	GameManager::transition_callback_function callback;
	ScreenFade* overlay;
};

