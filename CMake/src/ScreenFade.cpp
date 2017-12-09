#include "ScreenFade.h"
#include "Settings.h"

ScreenFade::ScreenFade() : Sprite(TextureManager::FADE_OVERLAY)
{
	drawOrder = 2147483646;
	size = Vector2i(SCREEN_WIDTH, SCREEN_HEIGHT);
}

void ScreenFade::Update()
{
	switch (fade)
	{
	case ScreenFader::FADE_IN:
		color.a -= 0x05;
		if (color.a <= 0x00)
		{
			color.a = 0x00;	
			ScreenFader::Instance().FinishFade();
		}

		break;

	case ScreenFader::FADE_OUT:
		color.a += 0x05;
		if (color.a >= 0xFF)
		{
			color.a = 0xFF;
			ScreenFader::Instance().FinishFade();
		}

		break;
	}
}
