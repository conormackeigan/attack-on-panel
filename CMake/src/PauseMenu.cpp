#include "PauseMenu.h"
#include "UIManager.h"
#include "UIText.h"
#include "InputController.h"
#include "GameManager.h"
#include "EntityManager.h"
#include "AudioManager.h"

#define ui_manager		 UIManager::Instance()
#define input_controller InputController::Instance()
#define game_manager	 GameManager::Instance()
#define entity_manager	 EntityManager::Instance()
#define audio_manager	 AudioManager::Instance()

PauseMenu::PauseMenu(int x, int y) : x(x), y(y)
{
}


PauseMenu::~PauseMenu()
{
}

void PauseMenu::Init()
{
	text_continue = ui_manager.Factory<UIText>();
	text_continue->text = "Continue";
	text_continue->position.x = x - FC_GetWidth(text_continue->GetFont(), text_continue->text.c_str()) / 2;
	text_continue->position.y = y - FC_GetHeight(text_continue->GetFont(), text_continue->text.c_str()) / 2 - 50;
	
	text_quit = ui_manager.Factory<UIText>();
	text_quit->text = "Quit Game";
	text_quit->position.x = text_continue->position.x;
	text_quit->position.y = y + FC_GetHeight(text_quit->GetFont(), text_quit->text.c_str()) / 2 - 48 ;

	pointer = ui_manager.Factory<UIText>();
	pointer->text = '>';
	pointer->position.x = text_continue->position.x - FC_GetWidth(pointer->GetFont(), pointer->text.c_str()) - 4;
	pointer->position.y = text_continue->position.y;
}

void PauseMenu::Update()
{
	if (done)
		return;

	if (input_controller.PressAction() || input_controller.PressCancel() || input_controller.PressStart())
	{
		audio_manager.PlaySFX(AudioManager::MENU_VALIDATE);

		switch (selection)
		{
		case Selection::CONTINUE:
			done = true;
			game_manager.ResumeGame();
			entity_manager.DestroyEntity(this);

			break;

		case Selection::QUIT:
			done = true;
			game_manager.QuitGame();

			break;
		}
	}

	if (input_controller.PressUp())
	{
		if (selection != CONTINUE)
		{
			audio_manager.PlaySFX(AudioManager::MOVEMENT);

			selection = CONTINUE;
			pointer->position.y = text_continue->position.y;
		}
	}
	else if (input_controller.PressDown())
	{
		if (selection != QUIT)
		{
			audio_manager.PlaySFX(AudioManager::MOVEMENT);

			selection = QUIT;
			pointer->position.y = text_quit->position.y;
		}
	}
}

void PauseMenu::OnDestroy()
{
	ui_manager.DestroyUIElement(text_continue);
	ui_manager.DestroyUIElement(text_quit);
	ui_manager.DestroyUIElement(pointer);
}
