#include "TimeAttackTimer.h"
#include "UIText.h"
#include "GameGrid.h"
#include "GameManager.h"

#define game_manager GameManager::Instance()

void TimeAttackTimer::Init()
{
	GameTimer::Init();

	seconds = 120;
	timer_text->text = "2'00";
}

void TimeAttackTimer::Update()
{
	if (!timer_on)
		return;

	if (++tick_count == 60)
	{
		--seconds;

		if (seconds == 0)
		{
			// TODO: end game
		}

		tick_count = 0;

		uint8_t minutes = seconds / 60;

		uint8_t secs = seconds % 60;
		std::string str_secs;
		if (secs > 9)
			str_secs = std::to_string(secs);
		else
			str_secs = '0' + std::to_string(secs);

		timer_text->text = std::to_string(minutes) + "'" + str_secs;
	}
}
