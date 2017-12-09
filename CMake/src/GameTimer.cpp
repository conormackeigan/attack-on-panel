#include "GameTimer.h"
#include "GameManager.h"
#include "UIManager.h"
#include "UIText.h"
#include "FontManager.h"

#define game_manager GameManager::Instance()
#define ui_manager UIManager::Instance()

void GameTimer::Init()
{
	timer_text = ui_manager.Factory<UIText>();

	tick_count = 0;
}

void GameTimer::OnDestroy()
{
	ui_manager.DestroyUIElement(timer_text);
}

void GameTimer::StartTimer()
{
	timer_on = true;
}

void GameTimer::PauseTimer()
{
	timer_on = false;
}

unsigned int GameTimer::GetTime()
{
	return seconds;
}
