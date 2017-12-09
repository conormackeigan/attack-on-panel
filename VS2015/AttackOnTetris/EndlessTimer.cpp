#include "EndlessTimer.h"
#include "UIText.h"
#include "GameGrid.h"
#include "GameManager.h"

#define game_manager GameManager::Instance()

void EndlessTimer::Init()
{
	GameTimer::Init();

	seconds = 0;
	timer_text->text = "00'00";
}

void EndlessTimer::Update()
{
	if (!timer_on || seconds >= 5999)
		return;

	if (++tick_count == 60)
	{
		++seconds;
		tick_count = 0;

		uint8_t minutes = seconds / 60;
		std::string str_minutes;
		if (minutes > 9)
			str_minutes = std::to_string(minutes);
		else
			str_minutes = '0' + std::to_string(minutes);

		uint8_t secs = seconds % 60;
		std::string str_secs;
		if (secs > 9)
			str_secs = std::to_string(secs);
		else
			str_secs = '0' + std::to_string(secs);

		timer_text->text = str_minutes + "'" + str_secs;
	}
}
