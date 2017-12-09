#include "ScreenFader.h"
#include "ScreenFade.h"
#include "EntityManager.h"
#include "GameManager.h"
#include "StandardTypes.h"

#define entity_manager EntityManager::Instance()

typedef void(*transition_callback_function)(void);

ScreenFader& ScreenFader::Instance()
{
	static ScreenFader instance;

	return instance;
}

void ScreenFader::FadeIn(GameManager::transition_callback_function callback)
{
	ASSERT(overlay == nullptr, "Attempting to fade in while one is already underway");

	this->callback = callback;

	overlay = entity_manager.Factory<ScreenFade>();
	overlay->fade = FADE_IN;
	overlay->color = { 0x00, 0x00, 0x00, 0xFF };
}

void ScreenFader::FadeOut(GameManager::transition_callback_function callback)
{
	ASSERT(overlay == nullptr, "Attempting to fade out while one is already underway");

	this->callback = callback;

	overlay = entity_manager.Factory<ScreenFade>();
	overlay->fade = FADE_OUT;
	overlay->color = { 0x00, 0x00, 0x00, 0x00 };
}

void ScreenFader::FinishFade()
{
	entity_manager.DestroyEntity(overlay);
	overlay = nullptr;

	if (callback != nullptr)
		callback();
}
