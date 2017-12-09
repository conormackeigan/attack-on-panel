#include "MainMenu.h"
#include "GameManager.h"
#include "AudioManager.h"
#include "UIManager.h"
#include "UIText.h"
#include "InputController.h"
#include "EntityManager.h"
#include "Settings.h"
#include "ColorCode.h"
#include <sstream>

#define game_manager		GameManager::Instance()
#define audio_manager		AudioManager::Instance()
#define ui_manager			UIManager::Instance()
#define input_controller	InputController::Instance()
#define entity_manager		EntityManager::Instance()

void MainMenu::Init()
{
	// Set up menu selector
	pointer_mode_select = ui_manager.Factory<UIText>();
	pointer_mode_select->text = '>';

	pointer_difficulty_select = ui_manager.Factory<UIText>();
	pointer_difficulty_select->text = '>';

	// Set up mode select menu (Endless / Time Attack)
	for (int i = 0; i < 3; i++)
		text_mode_select[i] = ui_manager.Factory<UIText>();

	text_mode_select_position[0] = Vector2f(SCREEN_WIDTH * 0.5f - 74, SCREEN_HEIGHT * 0.5f - TEXT_HEIGHT);
	text_mode_select_position[1] = Vector2f(SCREEN_WIDTH * 0.5f - 74, SCREEN_HEIGHT * 0.5f);
	text_mode_select_position[2] = Vector2f(SCREEN_WIDTH * 0.5f - 74, SCREEN_HEIGHT * 0.5f + TEXT_HEIGHT);

	text_mode_select[0]->text = "Endless";
	text_mode_select[1]->text = "Time Attack";
	text_mode_select[2]->text = "Quit";

	// Set up grid size select
	grid_size = { MIN_GRID_WIDTH, MIN_GRID_HEIGHT };

	text_grid_size_position = Vector2f(SCREEN_WIDTH * 0.5f - 96, SCREEN_HEIGHT + 2);
	panel_grid_position = Vector2f(SCREEN_WIDTH * 0.5f - Panel::SIZE * MAX_GRID_WIDTH * 0.5f, SCREEN_HEIGHT + 32);

	text_grid_size = ui_manager.Factory<UIText>();
	text_grid_size->text = "Grid Size: 6x12";

	PopulatePanelGrid();

	// Set up difficulty select menu (Easy, NORMAL, Hard, L+up+A for VHard)
	for (int i = 0; i < 3; i++)
		text_difficulty_select[i] = ui_manager.Factory<UIText>();

	text_difficulty_select_position[0] = Vector2f(SCREEN_WIDTH * 0.5f - 42, SCREEN_HEIGHT * 2 + (SCREEN_HEIGHT * 0.5f - TEXT_HEIGHT * 1.5f));
	text_difficulty_select_position[1] = Vector2f(SCREEN_WIDTH * 0.5f - 42, SCREEN_HEIGHT * 2 + SCREEN_HEIGHT * 0.5f - TEXT_HEIGHT * 0.5f);
	text_difficulty_select_position[2] = Vector2f(SCREEN_WIDTH * 0.5f - 42, SCREEN_HEIGHT * 2 + (SCREEN_HEIGHT * 0.5f + TEXT_HEIGHT * 0.5f));

	text_difficulty_select[0]->text = "Easy";
	text_difficulty_select[1]->text = "Normal";
	text_difficulty_select[2]->text = "Hard";
	
	// Set the correct render positions of all items
	UpdatePanelPositions();
}

void MainMenu::Update()
{
	if (scrolling)
	{
		scroll_offset.y += (desired_scroll_offset - scroll_offset.y) * 0.1f;
		UpdatePanelPositions();

		if (abs(desired_scroll_offset - scroll_offset.y) < 0.5f)
		{
			scroll_offset.y = desired_scroll_offset;
			UpdatePanelPositions();

			scrolling = false;
			
		}
	}

	// Too lazy to make a real FSM
	switch (state)
	{
	case SELECT_MODE:
		if (input_controller.PressUp())
		{
			if (selected_mode_select != 0)
			{
				--selected_mode_select;
				audio_manager.PlaySFX(AudioManager::SFX::MOVEMENT);
				pointer_mode_select->position = text_mode_select[selected_mode_select]->position - POINTER_MARGIN;
			}
		}
		else if (input_controller.PressDown())
		{
			if (selected_mode_select != 2)
			{
				++selected_mode_select;
				audio_manager.PlaySFX(AudioManager::SFX::MOVEMENT);
				pointer_mode_select->position = text_mode_select[selected_mode_select]->position - POINTER_MARGIN;
			}
		}

		if (input_controller.PressAction() || input_controller.PressStart())
		{
			// Quit game
			if (selected_mode_select == 2)
			{
				game_manager.QuitApp();
				return;
			}

			// Select game mode
			audio_manager.PlaySFX(AudioManager::SFX::MENU_VALIDATE);
			game_mode = (GameMode)selected_mode_select;
			TransitionState(State::SELECT_SIZE);
			
			break;
		}

		break;

	case SELECT_SIZE:
		if (input_controller.PressUp())
		{
			if (grid_size.y < MAX_GRID_HEIGHT)
			{
				++grid_size.y;

				UpdateGridSizeText();

				// Remove row of blank panels at top
				for (int i = 0; i < grid_size.x; i++)
				{
					panel_grid[i][MAX_GRID_HEIGHT - grid_size.y]->Recycle();
					panel_grid[i][MAX_GRID_HEIGHT - grid_size.y] = Panel::GetFromPool(grid_types[i][MAX_GRID_HEIGHT - grid_size.y]);
				}

				UpdatePanelPositions();
			}
		}
		else if (input_controller.PressDown())
		{
			if (grid_size.y > MIN_GRID_HEIGHT)
			{
				--grid_size.y;

				UpdateGridSizeText();

				// Remove row of colored panels at top
				for (int i = 0; i < grid_size.x; i++)
				{
					panel_grid[i][MAX_GRID_HEIGHT - grid_size.y - 1]->Recycle();
					panel_grid[i][MAX_GRID_HEIGHT - grid_size.y - 1] = Panel::GetFromPool(Panel::Blank);
				}

				UpdatePanelPositions();
			}
		}
		else if (input_controller.PressLeft())
		{
			if (grid_size.x > MIN_GRID_WIDTH)
			{
				// Remove column of colored panels on the right
				for (int i = MAX_GRID_HEIGHT - grid_size.y; i < MAX_GRID_HEIGHT; i++)
				{
					panel_grid[grid_size.x - 1][i]->Recycle();
					panel_grid[grid_size.x - 1][i] = Panel::GetFromPool(Panel::Blank);
				}

				--grid_size.x;

				UpdateGridSizeText();
				UpdatePanelPositions();
			}
		}
		else if (input_controller.PressRight())
		{
			if (grid_size.x < MAX_GRID_WIDTH)
			{
				// Remove column of blank panels on the right
				for (int i = MAX_GRID_HEIGHT - grid_size.y; i < MAX_GRID_HEIGHT; i++)
				{
					panel_grid[grid_size.x][i]->Recycle();
					panel_grid[grid_size.x][i] = Panel::GetFromPool(grid_types[grid_size.x][i]);
				}

				++grid_size.x;

				UpdateGridSizeText();
				UpdatePanelPositions();
			}
		}
		else if (input_controller.PressCancel())
		{
			audio_manager.PlaySFX(AudioManager::SFX::MENU_CANCEL);

			TransitionState(State::SELECT_MODE);
			break;
		}
		else if (input_controller.PressAction() || input_controller.PressStart())
		{
			audio_manager.PlaySFX(AudioManager::SFX::MENU_VALIDATE);

			TransitionState(State::SELECT_DIFFICULTY);
			break;
		}

		break;

	case SELECT_DIFFICULTY:
		if (input_controller.PressUp())
		{
			if (selected_difficulty_select != 0)
			{
				--selected_difficulty_select;
				audio_manager.PlaySFX(AudioManager::SFX::MOVEMENT);
				pointer_difficulty_select->position = text_difficulty_select[selected_difficulty_select]->position - POINTER_MARGIN;
			}
		}
		else if (input_controller.PressDown())
		{
			if (selected_difficulty_select != 2)
			{
				++selected_difficulty_select;
				audio_manager.PlaySFX(AudioManager::SFX::MOVEMENT);
				pointer_difficulty_select->position = text_difficulty_select[selected_difficulty_select]->position - POINTER_MARGIN;
			}
		}
		else if (input_controller.PressCancel())
		{
			audio_manager.PlaySFX(AudioManager::SFX::MENU_CANCEL);
			TransitionState(State::SELECT_SIZE);
			break;
		}
		else if (input_controller.PressAction() || input_controller.PressStart())
		{
			audio_manager.PlaySFX(AudioManager::SFX::MENU_VALIDATE);

			// Start game session with parameters
			difficulty = (Difficulty)selected_difficulty_select;

			// Classic throwback (TA uses Up+L+A... but this menu is navigated vertically, so it has to be right/left. Whoops)
			if (difficulty == Difficulty::HARD && input_controller.HoldLeftShoulder() && (input_controller.HoldLeft() || input_controller.HoldRight()))
				difficulty = Difficulty::VHARD;

			game_manager.StartGameSession(game_mode, difficulty, grid_size);
			state = CLOSING;
		}

		break;
	}
}

void MainMenu::OnDestroy()
{
	// Clean up
	for (int i = 0; i < 2; i++)
		ui_manager.DestroyUIElement(text_mode_select[i]);

	ui_manager.DestroyUIElement(text_grid_size);

	for (int i = 0; i < MAX_GRID_WIDTH; i++)
	{
		for (int j = 0; j < MAX_GRID_HEIGHT; j++)
		{
			panel_grid[i][j]->Recycle(); // recycle to pool
			panel_grid[i][j] = nullptr;
		}
	}

	for (int i = 0; i < 3; i++)
		ui_manager.DestroyUIElement(text_difficulty_select[i]);

	ui_manager.DestroyUIElement(pointer_mode_select);
	ui_manager.DestroyUIElement(pointer_difficulty_select);
}

void MainMenu::SetActive(bool active)
{
	this->active = active;

	// Janky manual parent hierarchy (no parenting system yet)
	if (active)
	{
		for (int i = 0; i < 2; i++)
			text_mode_select[i]->draw = true;

		text_grid_size->draw = true;

		PopulatePanelGrid();
		UpdatePanelPositions();

		for (int i = 0; i < 3; i++)
			text_difficulty_select[i]->draw = true;

		pointer_mode_select->draw = true;
		pointer_difficulty_select->draw = true;
	}
	else
	{
		for (int i = 0; i < 2; i++)
			text_mode_select[i]->draw = false;

		text_grid_size->draw = false;

		for (int i = 0; i < MAX_GRID_WIDTH; i++)
		{
			for (int j = 0; j < MAX_GRID_HEIGHT; j++)
			{
				panel_grid[i][j]->Recycle(); // recycle to pool
			}
		}

		for (int i = 0; i < 3; i++)
			text_difficulty_select[i]->draw = false;

		pointer_mode_select->draw = false;
		pointer_difficulty_select->draw = false;
	}
}

void MainMenu::PopulatePanelGrid()
{
	Panel::PanelType curr_type = (Panel::PanelType)(Panel::Blank + 1);
	for (int i = 0; i < MAX_GRID_WIDTH; i++)
	{
		for (int j = 0; j < MAX_GRID_HEIGHT; j++)
		{
			grid_types[i][j] = curr_type;

			// Hardcode default to classic size (6x12)
			if (j < MAX_GRID_HEIGHT - grid_size.y || i >= grid_size.x)
				panel_grid[i][j] = Panel::GetFromPool(Panel::Blank);
			else
				panel_grid[i][j] = Panel::GetFromPool(curr_type);

			curr_type = (Panel::PanelType)((int)curr_type + 1);

			if (curr_type == Panel::NUM_PANEL_TYPES)
				curr_type = (Panel::PanelType)(Panel::Blank + 1); // Skip Blank
		}
	}
}

void MainMenu::TransitionState(State new_state)
{
	scrolling = true;
	desired_scroll_offset = state_scroll_offset[new_state];
	state = new_state;
}

void MainMenu::UpdatePanelPositions()
{
	text_mode_select[0]->position = text_mode_select_position[0] - scroll_offset;
	text_mode_select[1]->position = text_mode_select_position[1] - scroll_offset;
	text_mode_select[2]->position = text_mode_select_position[2] - scroll_offset;

	text_grid_size->position = text_grid_size_position - scroll_offset;
	
	for (int i = 0; i < MAX_GRID_WIDTH; i++)
	{
		for (int j = 0; j < MAX_GRID_HEIGHT; j++)
		{
			panel_grid[i][j]->position = (panel_grid_position - scroll_offset) + Vector2f(i * Panel::SIZE, j * Panel::SIZE);
		}
	}

	text_difficulty_select[0]->position = text_difficulty_select_position[0] - scroll_offset;
	text_difficulty_select[1]->position = text_difficulty_select_position[1] - scroll_offset;
	text_difficulty_select[2]->position = text_difficulty_select_position[2] - scroll_offset;

	// Pointers (selectors)
	pointer_mode_select->position = text_mode_select[selected_mode_select]->position - POINTER_MARGIN;
	pointer_difficulty_select->position = text_difficulty_select[selected_difficulty_select]->position - POINTER_MARGIN;
}

void MainMenu::UpdateGridSizeText()
{
	std::stringstream ss;
	ss << "Grid Size: " << grid_size.x << "x" << grid_size.y;
	text_grid_size->text = ss.str();
}
