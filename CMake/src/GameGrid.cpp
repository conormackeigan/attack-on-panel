#pragma warning (disable:4018) // Signed/unsigned mismatch

#include "GameGrid.h"
#include <math.h>
#include "StandardTypes.h"
#include "EntityManager.h"
#include "InputController.h"
#include "UIManager.h"
#include "ColorCode.h"
#include "UIText.h"
#include "Swapper.h"
#include "EndlessTimer.h"
#include "TimeAttackTimer.h"
#include "FontManager.h"
#include "AudioManager.h"
#include "PauseMenu.h"
#include <iterator>
#include <algorithm>
#include "GameManager.h"
#include "LogSpiralParticle.h"
#include "ComboSign.h"
#include "ChainSign.h"

#define entity_manager	 EntityManager::Instance()
#define input_controller InputController::Instance()
#define ui_manager		 UIManager::Instance()
#define audio_manager	 AudioManager::Instance()

#define WAIT_TIME 40 // Number of frames to wait before fading in initial panels

#define LOOP_GRID for (int x = 0; x < grid_size.x; x++) for (int y = 0; y < grid_size.y; y++)

const Vector2f GameGrid::SWAPPER_OFFSET = { -5.f, -2.f };

GameGrid::GameGrid(GameMode game_mode, Vector2i grid_size, Difficulty difficulty) : game_mode(game_mode) ,grid_size(grid_size), difficulty(difficulty), risen(0), rise_timer(0)
{	
}

void GameGrid::Init()
{
	// Initialize grid
	grid.reserve(grid_size.x);
	for (int i = 0; i < grid_size.x; i++)
	{
		grid.push_back(std::vector<Panel*>());

		grid[i].reserve(grid_size.y);
		for (int j = 0; j < grid_size.y; j++)
		{
			grid[i].push_back(Panel::GetFromPool(Panel::Empty));
		}
	}

	next_row.reserve(grid_size.x);
	for (int i = 0; i < grid_size.x; i++)
		next_row.push_back(nullptr);

	// Store grid info
	grid_pixel_size = Vector2i(Panel::SIZE * grid_size.x, Panel::SIZE * grid_size.y);

	float x = SCREEN_WIDTH * 0.5f - (grid_pixel_size.x) * 0.5f;
	float y = SCREEN_HEIGHT * 0.5f - (grid_pixel_size.y) * 0.5f;

	grid_origin = Vector2f(x, y);

	// Populate grid
	Generate();

	// Initialize border
	for (int i = 0; i < 4; i++)
	{
		border[i] = entity_manager.Factory<Sprite>();
		border[i]->color = ColorCode::BROWN;
	}

	// Top border
	border[0]->position = Vector2f(grid_origin.x - BORDER_THICKNESS, grid_origin.y - BORDER_THICKNESS);
	border[0]->size = Vector2f(grid_pixel_size.x + BORDER_THICKNESS * 2, BORDER_THICKNESS);

	// Right border
	border[1]->position = Vector2f(grid_origin.x + grid_pixel_size.x, grid_origin.y);
	border[1]->size = Vector2f(BORDER_THICKNESS, grid_pixel_size.y);

	// Bottom border
	border[2]->position = Vector2f(grid_origin.x - BORDER_THICKNESS, grid_origin.y + grid_pixel_size.y);
	border[2]->size = Vector2f(grid_pixel_size.x + BORDER_THICKNESS * 2, BORDER_THICKNESS);

	// Left border
	border[3]->position = Vector2f(grid_origin.x - BORDER_THICKNESS, grid_origin.y);
	border[3]->size = Vector2f(BORDER_THICKNESS, grid_pixel_size.y);

	// Initialize UI (here be spaghetti; refactor if the game ever is to get more complex than 2 modes)
	switch (game_mode)
	{
	case ENDLESS:
		// Health
		max_health = 1;
		health = max_health;

		// Timer 
		game_timer = entity_manager.Factory<EndlessTimer>(); 
		game_timer->timer_text->position = Vector2f(grid_origin.x - BORDER_THICKNESS - FC_GetWidth(game_timer->timer_text->GetFont(), game_timer->timer_text->text.c_str()) - 2, grid_origin.y);

		// Score
		score_text = ui_manager.Factory<UIText>();
		score_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		score_text->text = "SCORE:\n0";
		score_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 0.f);

		// Speed level
		speed_level_text = ui_manager.Factory<UIText>();
		speed_level_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		speed_level_text->text = "LEVEL 1";
		speed_level_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 54.f);

		// Difficulty
		difficulty_text = ui_manager.Factory<UIText>();
		difficulty_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		difficulty_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 94.f);

		// Stop
		stop_text = ui_manager.Factory<UIText>();
		stop_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		stop_text->position = speed_level_text->position;
		stop_text->draw = false;

		switch (difficulty)
		{
		case EASY:
			difficulty_text->text = "EASY";
			break;

		case NORMAL:
			difficulty_text->text = "NORMAL";
			break;

		case HARD:
			difficulty_text->text = "HARD";
			break;

		case VHARD:
			difficulty_text->text = "VERY HARD";
			break;
		}

		break;

	case TIME_ATTACK:
		// Health
		max_health = 1;
		health = max_health;

		// Timer
		game_timer = entity_manager.Factory<TimeAttackTimer>();
		game_timer->timer_text->position = Vector2f(grid_origin.x - BORDER_THICKNESS - FC_GetWidth(game_timer->timer_text->GetFont(), game_timer->timer_text->text.c_str()) - 2, grid_origin.y);

		// Score
		score_text = ui_manager.Factory<UIText>();
		score_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		score_text->text = "SCORE:\n0";
		score_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 0.f);

		// Speed level
		speed_level_text = ui_manager.Factory<UIText>();
		speed_level_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		speed_level_text->text = "LEVEL 1";
		speed_level_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 54.f);

		// Stop
		stop_text = ui_manager.Factory<UIText>();
		stop_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		stop_text->position = speed_level_text->position;
		stop_text->draw = false;

		// Difficulty
		difficulty_text = ui_manager.Factory<UIText>();
		difficulty_text->SetFont(FontManager::FONT_NAME::VERA_SE_14);
		difficulty_text->position = grid_origin + Vector2f(grid_pixel_size.x + BORDER_THICKNESS + 2, 94.f);	

		switch (difficulty)
		{
		case EASY:
			difficulty_text->text = "EASY";
			break;

		case NORMAL:
			difficulty_text->text = "NORMAL";
			break;

		case HARD:
			difficulty_text->text = "HARD";
			break;

		case VHARD:
			difficulty_text->text = "VERY HARD";
			break;
		}

		break;
	}
}

void GameGrid::Update()
{
	// DEBUG: print panel info on click
#ifndef NDEBUG
	if (input_controller.MouseLeftClick())
	{
		Panel* click = GetPanelAtWorldPos(input_controller.MousePosition());
		if (click != nullptr)
			std::cout << click->ToString() << std::endl;
	}
#endif

	// GLOBAL STATE (panel shake)
	bouncer.Update();

	// Who needs a state machine when you have big ugly switch statements
	switch (state)
	{
	case WAIT:
		// Wait 1 sec
		if (++state_time < WAIT_TIME)
			return;

		ChangeState(FADEIN_PANELS);

		break;

	case FADEIN_PANELS:
		for (int i = 0; i < grid_size.x; i++)
		{
			for (int j = 0; j < grid_size.y; j++)
			{
				if (grid[i][j]->panel_type == Panel::Empty)
					continue;

				// NOTE: SDL_Color uses 8-bit ints, so some additional checks will be required if the increment is not divisible by 0xFF
				grid[i][j]->color.r += 0x05;
				grid[i][j]->color.g += 0x05;
				grid[i][j]->color.b += 0x05;
			}
		}

		if (grid[0][grid_size.y - 1]->color.r == 0xFF)
		{
			ChangeState(READY_DROP);
		}

		break;

	case READY_DROP:
		// Drop down READY
		ready_text->position.y += 3;
		
		if (ready_text->position.y >= ready_target_y)
		{
			ready_text->position.y = ready_target_y;
			ChangeState(COUNTDOWN);
		}

		break;

	case COUNTDOWN:
		if (++state_time == 60)
		{
			state_time = 0;

			if (--countdown_value > 0)
			{
				//audio_manager.PlaySFX(AudioManager::SFX::COUNTDOWN);

				countdown_text->text = std::to_string(countdown_value);
			}
			else
			{
				ui_manager.DestroyUIElement(ready_text);
				ui_manager.DestroyUIElement(countdown_text);

				//audio_manager.PlaySFX(AudioManager::SFX::GO);
				audio_manager.PlayMusic(AudioManager::WATER_STAGE, 0, 500);

				ChangeState(PLAY);

				break;			
			}		
		}

		// Move swapper to center
		if (!unlock_swapper)
		{
			if (swapper->position.y < swapper_target.y)
			{
				swapper->position.y += 10;

				if (swapper->position.y > swapper_target.y)
					swapper->position.y = swapper_target.y;
			}
			else if (swapper->position.x > swapper_target.x)
			{
				swapper->position.x -= 10;

				if (swapper->position.x < swapper_target.x)
				{
					// Reached starting position
					swapper->position.x = swapper_target.x;
					swapper->grid_pos = Vector2i(grid_size.x / 2 - 1, ceil(grid_size.y * 0.5f));
					unlock_swapper = true;
				}
			}
		}
		else
		{	// Swapper movement unlocked during countdown
			if (input_controller.PressRight())
			{
				// todo: swapper SFX
				swapper->grid_pos.x = std::min(swapper->grid_pos.x + 1, grid_size.x - 2);
				UpdateSwapperPosition();
			}
			else if (input_controller.PressLeft())
			{
				// todo: swapper SFX
				swapper->grid_pos.x = std::max(swapper->grid_pos.x - 1, 0);
				UpdateSwapperPosition();
			}

			if (input_controller.PressUp())
			{
				// todo: swapper SFX
				// Damage flag indicates that there are panels at the top, so use it to allow top-row access
				swapper->grid_pos.y = std::max(swapper->grid_pos.y - 1, 1);
				UpdateSwapperPosition();
			}
			else if (input_controller.PressDown())
			{
				// todo: swapper SFX
				swapper->grid_pos.y = std::min(swapper->grid_pos.y + 1, grid_size.y - 1);
				UpdateSwapperPosition();
			}
		}

		break;

	case PLAY:
	{
		// Check for timeout
		if (game_mode == TIME_ATTACK)
		{
			if (game_timer->GetTime() == 0)
			{
				ChangeState(GAME_OVER);
				game_timer->PauseTimer();
			}
		}

		// Reset landing SFX flag
		panel_landed = false;
		combo_or_chain = false;
		stop_this_frame = false;

		//--------------------------------------------------------------------------
		// Grid rising
		//--------------------------------------------------------------------------
		// Force rise with shoulder button
		if ((input_controller.HoldLeftShoulder() || input_controller.HoldRightShoulder()) && clears.empty())
		{
			force_rise = true;
			CancelStop();
		}

		if (stop_timer > 0)
		{
			TickStop();
		}
		else if (clears.size() == 0) // Grid doesn't rise while clears are active
		{
			damage = false;
			for (int i = 0; i < grid_size.x; i++)
			{
				// If a top column is full and the stack wants to rise a pixel, reduce health
				if (grid[i][0]->panel_type != Panel::Empty)
				{
					damage = true;
					break;
				}
			}

			if (damage)
			{
				if (--health == 0)
				{
					GameOver();
					return;
				}
			}
			else
			{
				health = max_health;
			}
			
			if (force_rise)
			{
				if (++rise_timer > 2)
				{
					risen += 2;
					if (risen > Panel::SIZE + 1)
					{
						RaiseGrid();

						risen = 0;
						rise_timer = 0;
						force_rise = false;
					}

					UpdatePanelPositions();
				}
			}
			else
			{
				// Rise
				if (++rise_timer >= ceil(speed_to_rise_time[speed_level] / speed_to_rise_difficulty_modifier[difficulty]) / 2.f)
				{
					rise_timer = 0;

					if (++risen == Panel::SIZE)
					{
						risen = 0;
						RaiseGrid();
					}

					UpdatePanelPositions();
				}
			}		

			
		}

		// Check if in danger (TODO: panic panel sprites)
		bool in_danger = false;
		for (int i = 0; i < grid_size.x; i++)
		{
			if (grid[i][2]->panel_type != Panel::Empty)
			{
				in_danger = true;
				break;
			}
		}

		if (in_danger)
		{
			if (!danger)
			{
				danger = true;
				audio_manager.PlayMusic(AudioManager::WATER_DANGER, 25, 25);
			}
		}
		else if (danger)
		{
			danger = false;
			audio_manager.PlayMusic(AudioManager::WATER_STAGE, 25, 25);
		}

		// Pause
		if (input_controller.PressStart())
		{
			ChangeState(PAUSE);
			audio_manager.PlaySFX(AudioManager::MOVEMENT);
			audio_manager.PauseMusic();

			break;
		}

		//-------------------------------------------------------
		// Swapper logic
		//-------------------------------------------------------
		SwapperInput();

		// Process swap if there is one
		TickSwap();

		// Make panels fall if there is empty space beneath them
		ApplyGravity();	
		
		// Process any clears that are a result of swaps or falls
		ProcessClears();

		// SFX
		if (stop_this_frame)
			audio_manager.PlaySFX(AudioManager::STOP);
		else if (combo_or_chain)
			audio_manager.PlaySFX(AudioManager::COMBO_CHAIN_GET);
		else if (panel_landed)
			audio_manager.PlaySFX(AudioManager::PANEL_LAND);

		break;
	}

	case GAME_OVER:
		// shake shake
		UpdatePanelPositions();

		fade_value = std::min(0xFF, fade_value + 0x05);

		if (panels_dying)
		{
			LOOP_GRID
			{
				if (grid[x][y]->panel_type != Panel::Empty)
					grid[x][y]->dead_sprite->color.a = fade_value;
				
			if (fade_value == 0xFF)
				panels_dying = false;
			}
		}
		// Wait a little after all the panels are dead then fade them away
		else if (!fading_out)
		{
			if (++state_time > 12)
			{
				fading_out = true;
				state_time = 0;
			}
		}
		else
		{
			if (fade_out_row < grid_size.y - 1)
			{
				if (++state_time > 12)
				{
					state_time = 0;
					++fade_out_row;
				}
			}

			for (int x = 0; x < grid_size.x; x++)
			{
				for (int y = grid_size.y - 1; y > grid_size.y - 2 - fade_out_row; y--)
				{
					if (grid[x][y]->panel_type != Panel::Empty)
					{
						grid[x][y]->color.a -= std::min((int)grid[x][y]->color.a, 0x05);
						grid[x][y]->dead_sprite->color.a -= std::min((int)grid[x][y]->dead_sprite->color.a, 0x0F);
					}
				}
			}
			
			if (++state_time > 65)
			{
				audio_manager.StopMusic(500);
				GameManager::Instance().QuitGame();
				state = (State)-1;
			}
		}
		
		break;

	case PAUSE:
		// Quickly alpha out grid
		if (!unpausing)
		{
			if (swapper->color.a > 0x00)
			{
				// This system is incredibly fragile - if increments are not divisible by 0xFF it'll loop for a very long time,
				// and it depends on the swapper and panel alphas always being synced (which they are - it's just not a robust system)
				swapper->color.a -= 0x0F;

				for (int i = 0; i < grid_size.x; i++)
				{
					next_row[i]->color.a -= 0x0F;

					for (int j = 0; j < grid_size.y; j++)
					{
						if (grid[i][j]->panel_type == Panel::Empty)
							continue;

						grid[i][j]->color.a -= 0x0F;
					}
				}
			}
			else if (pause_menu == nullptr)
			{
				// Finished fading out, display pause menu
				pause_menu = entity_manager.Factory<PauseMenu>(grid_origin.x + grid_pixel_size.x / 2, grid_origin.y + grid_pixel_size.y / 2);
			}
		}

		else
		{
			swapper->color.a += 0x0F;

			for (int i = 0; i < grid_size.x; i++)
			{
				next_row[i]->color.a += 0x0F;

				for (int j = 0; j < grid_size.y; j++)
				{
					if (grid[i][j]->panel_type == Panel::Empty)
						continue;

					grid[i][j]->color.a += 0x0F;
				}
			}

			if (swapper->color.a == 0xFF)
			{
				unpausing = false;
				pause_menu = nullptr;

				ChangeState(PLAY);
			}
		}

		break;
	}
}

void GameGrid::OnDestroy()
{
	// Destroy swapper
	entity_manager.DestroyEntity(swapper);

	// Delete panels (recycle to pool)
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = 0; j < grid_size.y; j++)
		{
			grid[i][j]->Recycle();
		}

		next_row[i]->Recycle();
	}

	// Delete UI and borders
	entity_manager.DestroyEntity(game_timer);

	ui_manager.DestroyUIElement(score_text);
	ui_manager.DestroyUIElement(speed_level_text);
	ui_manager.DestroyUIElement(difficulty_text);
	ui_manager.DestroyUIElement(stop_text);

	for (int i = 0; i < 4; i++)
		entity_manager.DestroyEntity(border[i]);

	if (pause_menu != nullptr)
		entity_manager.DestroyEntity(pause_menu);
}

void GameGrid::StartGame()
{
	state = WAIT;
}

Vector2f GameGrid::GetGridOrigin()
{
	return grid_origin;
}

Vector2f GameGrid::GetPixelSize()
{
	return grid_pixel_size;
}

void GameGrid::ChangeState(State new_state)
{
	// Exit old state
	switch (state)
	{
	case PAUSE:
		if (new_state == PLAY)
		{
			audio_manager.ResumeMusic();
			UnpausePanels();
		}

		break;
	}

	state_time = 0;
	state = new_state;

	// Enter new state
	switch (state)
	{
	case FADEIN_PANELS:
		// Set panels visible
		for (int i = 0; i < grid_size.x; i++)
		{
			for (int j = 0; j < grid_size.y; j++)
			{
				if (grid[i][j]->panel_type == Panel::Empty)
					continue;

				grid[i][j]->color.a = 0xFF;
			}
		}

		break;

	case READY_DROP:
		// Create READY text
		ready_text = ui_manager.Factory<UIText>();
		ready_text->text = "READY";
		
		// Center READY text and put just above the screen
		ready_text->position.x = SCREEN_WIDTH * 0.5f - FC_GetWidth(ready_text->GetFont(), ready_text->text.c_str()) * 0.5f;
		ready_text->position.y = -FC_GetHeight(ready_text->GetFont(), ready_text->text.c_str());

		// Store target y position so it doesn't have to be calculated every dang frame
		ready_target_y = SCREEN_HEIGHT * 0.5f - FC_GetHeight(ready_text->GetFont(), ready_text->text.c_str()) * 3.5f - 2;

		break;

	case COUNTDOWN:
		// Countdown text
		countdown_text = ui_manager.Factory<UIText>();
		countdown_text->text = '3';

		// Position just below ready, centered on x-axis
		countdown_text->position.x = SCREEN_WIDTH * 0.5f - FC_GetWidth(countdown_text->GetFont(), countdown_text->text.c_str()) * 0.5f;
		countdown_text->position.y = ready_target_y + FC_GetHeight(ready_text->GetFont(), ready_text->text.c_str()) + 2;
		
		countdown_value = 3;

		// Drop swapper in during countdown
		swapper = entity_manager.Factory<Swapper>();
		swapper->position.x = grid_origin.x + Panel::SIZE * (grid_size.x - 2);
		swapper->position.y = -swapper->size.y;

		// Target is the very middle of the grid, rounding to the left/bottom
		swapper_target = SWAPPER_OFFSET + Vector2f(grid_origin.x + (grid_size.x / 2 - 1) * Panel::SIZE, grid_origin.y + (ceil(grid_size.y * 0.5f)) * Panel::SIZE);

		//audio_manager.PlaySFX(AudioManager::SFX::COUNTDOWN);

		break;

	case PLAY:
		game_timer->StartTimer();

		break;

	case GAME_OVER:
		swapper->SetActive(false);
		bouncer.StartBounce();

		// Prepare panels for blackout
		panels_dying = true;
		for (int i = 0; i < grid_size.x; i++)
		{
			for (int j = 0; j < grid_size.y; j++)
			{
				if (grid[i][j]->panel_type != Panel::Empty)
				{
					grid[i][j]->Die();	
				}
			}
		}

		break;

	case PAUSE:
		game_timer->PauseTimer();
		PausePanels();

		break;
	}
}

void GameGrid::Generate()
{
	// Initial panel count is half-full minus (almost) one row
	unsigned int num_panels = (grid_size.x * grid_size.y) / 2 - grid_size.x;

	// Max initial column height
	unsigned int max_height = (unsigned int)ceil(grid_size.y * 0.5f);

	// Iterate over all columns with a 30% chance of dropping a randomly-colored panel
	// If the desired number of panels isn't reached, the remainder are hard-set
	unsigned int panel_count = 0;

	for (unsigned int i = 0; i < num_panels * 2; i++)
	{
		// Skip columns that are at max height
		if (grid[i % grid_size.x][grid_size.y - max_height]->panel_type != Panel::Empty)
			continue;

		if (randFloat() >= 0.3f)
		{
			DropInRandPanel(i % grid_size.x);

			if (++panel_count == num_panels)
				break;
		}
	}

	// Hard-code remaining panels if random gen didn't meet the quota
	while (panel_count < num_panels)
	{
		unsigned int panel_x = randRangeInt(0, grid_size.x); // TEST ME

		while (grid[panel_x][max_height]->panel_type != Panel::Empty)
		{
			panel_x = (panel_x + 1) % grid_size.x;
		}

		DropInRandPanel(panel_x);
	}

	// Panels start invisible, then mod-fade in from black
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = 0; j < grid_size.y; j++)
		{
			if (grid[i][j]->panel_type == Panel::Empty)
				continue;

			grid[i][j]->color = { 0x00, 0x00, 0x00, 0x00 };
		}
	}

	// Generate next row (dimmed at bottom of stack)
	GenerateNextRow();
}

void GameGrid::GenerateNextRow()
{
	uint8_t consecutive_same = 0;

	for (int i = 0; i < grid_size.x; i++)
	{
		Panel::PanelType rand_panel_type = (Panel::PanelType)randRangeInt((unsigned int)Panel::PanelType::Red, difficulty == EASY ? (unsigned int)Panel::NUM_PANEL_TYPES - 1 : (unsigned int)Panel::NUM_PANEL_TYPES);

		if (i > 0 ? next_row[i - 1]->panel_type == rand_panel_type : false)
			++consecutive_same;
		else
			consecutive_same = 0;

		if (consecutive_same == 2)
		{
			// Randomly generated three of the same color in a row, cycle to next color
			rand_panel_type = (Panel::PanelType)((unsigned int)((rand_panel_type + 1) % (difficulty == EASY ? (unsigned int)(Panel::NUM_PANEL_TYPES - 1) : (unsigned int)Panel::NUM_PANEL_TYPES)));
			if (rand_panel_type < Panel::Red) // hard-coded ravioli
				rand_panel_type = Panel::Red;
		}

		next_row[i] = Panel::GetFromPool(rand_panel_type);

		// Dim panels in new row
		next_row[i]->color = { 0x50, 0x50, 0x50, 0xFF };

		// Position beneath the stack
		next_row[i]->position.x = grid_origin.x + i * Panel::SIZE;
		next_row[i]->position.y = grid_origin.y + grid_size.y * Panel::SIZE;

		// The drawn height is proportional to how high this cycle has risen
		next_row[i]->src.h = 0;
		next_row[i]->size.y = 0;
	}
}

void GameGrid::AddPanel(Panel::PanelType panel_type, unsigned int x, unsigned int y)
{
	grid[x][y]->Recycle();

	Panel* panel = Panel::GetFromPool(panel_type);
	grid[x][y] = panel;
}

void GameGrid::RemovePanel(unsigned int x, unsigned int y)
{
	Panel* to_remove = grid[x][y];
	grid[x][y]->Recycle();
	grid[x][y] = Panel::GetFromPool(Panel::Empty);
}

void GameGrid::RemovePanel(Panel* panel)
{
	// FOR DEBUG ONLY, DO NOT USE IN ACTUAL GAME CODE
	panel->panel_type = Panel::Empty;
}

void GameGrid::DropInRandPanel(unsigned int x)
{
	// Find first empty y-pos in column
	int y = 0;

	while (grid[x][y + 1]->panel_type == Panel::Empty)
	{
		if (++y >= grid_size.y - 1)
			break;
	}

	// Select a random panel type and cycle if it results in a clear (3 in a row)
	Panel::PanelType rand_panel_type = (Panel::PanelType)randRangeInt((unsigned int)Panel::PanelType::Red, difficulty == EASY ? (unsigned int)Panel::NUM_PANEL_TYPES - 1 : (unsigned int)Panel::NUM_PANEL_TYPES);

	while (CheckSpawnClear(x, y, rand_panel_type))
	{
		rand_panel_type = (Panel::PanelType)((unsigned int)((rand_panel_type + 1) % (difficulty == EASY ? (unsigned int)(Panel::NUM_PANEL_TYPES - 1) : (unsigned int)Panel::NUM_PANEL_TYPES)));
		if (rand_panel_type < Panel::Red) // hard-coded ravioli
			rand_panel_type = Panel::Red;
	}

	AddPanel(rand_panel_type, x, y);

	grid[x][y]->position = grid_origin + Vector2f(x * Panel::SIZE, y * Panel::SIZE);
}

bool GameGrid::CheckSpawnClear(unsigned int x, unsigned int y, Panel::PanelType panel_type)
{
	// Panels are dropped in from top-bottom, so don't need to check above
	// Below:
	unsigned int yCount = 0;
	int index = y + 1;

	while (index < grid_size.y ? (grid[x][index]->panel_type == Panel::Empty ? false : grid[x][index]->panel_type == panel_type) : false)
	{
		++yCount;
		++index;
	}

	if (yCount > 1)
		return true;

	// Left / Right:
	unsigned int xCount = 0;
	
	// Left
	index = x - 1;
	while (index >= 0 ? (grid[index][y]->panel_type == Panel::Empty ? false : grid[index][y]->panel_type == panel_type) : false)
	{
		++xCount;
		--index;
	}

	// Right
	index = x + 1;
	while (index < grid_size.x ? (grid[index][y]->panel_type == Panel::Empty ? false : grid[index][y]->panel_type == panel_type) : false)
	{
		++xCount;
		++index;
	}

	if (xCount > 1)
		return true;

	return false;
}



void GameGrid::SwapperInput()
{
	if (input_controller.PressRight())
	{
		// todo: swapper SFX
		swapper->grid_pos.x = std::min(swapper->grid_pos.x + 1, grid_size.x - 2);
		UpdateSwapperPosition();
	}
	else if (input_controller.PressLeft())
	{
		// todo: swapper SFX
		swapper->grid_pos.x = std::max(swapper->grid_pos.x - 1, 0);
		UpdateSwapperPosition();
	}

	if (input_controller.PressUp())
	{
		// todo: swapper SFX
		// Damage flag indicates that there are panels at the top, so use it to allow top-row access
		swapper->grid_pos.y = std::max(swapper->grid_pos.y - 1, damage ? 0 : 1);
		UpdateSwapperPosition();
	}
	else if (input_controller.PressDown())
	{
		// todo: swapper SFX
		swapper->grid_pos.y = std::min(swapper->grid_pos.y + 1, grid_size.y - 1);
		UpdateSwapperPosition();
	}

	// Swapping
	if (input_controller.PressAction() || input_controller.PressCancel())
		// Panels to be swapped cannot be in certain states
		if (grid[swapper->grid_pos.x][swapper->grid_pos.y]->CanSwap() && grid[swapper->grid_pos.x + 1][swapper->grid_pos.y]->CanSwap())
			// Cannot swap directly beneath hanging panels
			if (swapper->grid_pos.y > 0 ? grid[swapper->grid_pos.x][swapper->grid_pos.y - 1]->state != Panel::Hovering && grid[swapper->grid_pos.x + 1][swapper->grid_pos.y - 1]->state != Panel::Hovering : true)
				StartSwap();
}

void GameGrid::StartSwap()
{
	// Cannot swap two empties
	if (grid[swapper->grid_pos.x][swapper->grid_pos.y]->panel_type == Panel::Empty && grid[swapper->grid_pos.x + 1][swapper->grid_pos.y]->panel_type == Panel::Empty)
		return;

	audio_manager.PlaySFX(AudioManager::SWAP);

	if (curr_swap.curr_time <= curr_swap.swap_time)
	{
		// There is already an active swap, force it to end
		FinishSwap();
	}

	curr_swap.swap_root = swapper->grid_pos;

	curr_swap.left = grid[swapper->grid_pos.x][swapper->grid_pos.y];
	curr_swap.right = grid[swapper->grid_pos.x + 1][swapper->grid_pos.y];

	curr_swap.left->prev_state = curr_swap.left->state;
	curr_swap.left->state = Panel::Swapping;

	curr_swap.right->prev_state = curr_swap.right->state;
	curr_swap.right->state = Panel::Swapping;

	curr_swap.left->hover_time = 0;
	curr_swap.right->hover_time = 0;

	curr_swap.curr_time = 0;
}

void GameGrid::TickSwap()
{
	// Sentinel value for no active swap
	if (curr_swap.curr_time > curr_swap.swap_time)
		return;

	if (++curr_swap.curr_time <= curr_swap.swap_time)
	{
		// Move panels through each other
		curr_swap.left->position.x  += Panel::SIZE / curr_swap.swap_time;	
		curr_swap.right->position.x -= Panel::SIZE / curr_swap.swap_time;
	}
	else
	{
		FinishSwap();
	}
}

void GameGrid::FinishSwap()
{
	// Set tick sentinel value (curr_time > swap_time = inactive swap)
	curr_swap.curr_time = curr_swap.swap_time + 1;

	// Swap data (except can_chain and hover_time)
	// NOTE: we need to preserve grid pointers for chaining logic
	Panel::Swap(*curr_swap.left, *curr_swap.right);

	// Reset states
	curr_swap.left->state = curr_swap.left->prev_state;
	curr_swap.right->state = curr_swap.right->prev_state;

	UpdatePanelPosition(curr_swap.swap_root.x, curr_swap.swap_root.y);
	UpdatePanelPosition(curr_swap.swap_root.x + 1, curr_swap.swap_root.y);

	// This is ravioli but it's the easiest way to prevent a panel that's about to enter hover state from clearing on the frame the swap completes... for now
	if (curr_swap.left->CanClear() && curr_swap.left->panel_type != Panel::Empty && (curr_swap.swap_root.y + 1 < grid_size.y - 1 ? grid[curr_swap.swap_root.x][curr_swap.swap_root.y + 1]->panel_type != Panel::Empty : true))
		panels_to_check_for_clears.insert(curr_swap.left);

	if (curr_swap.right->CanClear() && curr_swap.right->panel_type != Panel::Empty && (curr_swap.swap_root.y + 1 < grid_size.y - 1 ? grid[curr_swap.swap_root.x + 1][curr_swap.swap_root.y + 1]->panel_type != Panel::Empty : true))
		panels_to_check_for_clears.insert(curr_swap.right);
}


void GameGrid::ApplyGravity()
{
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = grid_size.y - 1; j > 0; j--)
		{
			// Search for the lowest unprocessed empty space in this column (mid-swap empties are solid)
			if (grid[i][j]->panel_type == Panel::Empty && grid[i][j]->state != Panel::Swapping && j > 0)
			{
				--j;

				// Found an empty panel, process above panels. Keep searching for a solid if above is empty.
				while (grid[i][j]->CanFall())
				{
					// Skip empty spaces until we find something solid in this column
					if (grid[i][j]->panel_type == Panel::Empty)
					{
						if (--j >= 0) 
							continue; // Do not process an empty panel for falling; search upward for a solid panel
						else
							break; // Reached the top of the column; move on to the next column (for i)
					}

					// Found a solid panel, apply gravity
					if (++grid[i][j]->hover_time > time_hover[difficulty])
						grid[i][j]->SetState(Panel::Falling);
					else
						grid[i][j]->SetState(Panel::Hovering);

					// If this panel is falling, process the y-position increment
					if (grid[i][j]->hover_time > time_hover[difficulty])
					{
						// Check if there is solid ground beneath this panel
						if (j < grid_size.y - 1 ? grid[i][j + 1]->panel_type != Panel::Empty : true)
						{
							// There is a solid panel below so this panel cannot fall, search upward for empties again
							if (--j >= 0)
								continue;
							else
								break;
						}

						// This panel needs to fall down
						Panel* temp = grid[i][j];
						grid[i][j] = grid[i][j + 1];
						grid[i][j + 1] = temp;
						UpdatePanelPosition(i, j + 1);						
					}

					// Panel is still hanging, continue traversing up the column
					if (--j < 0)
						break;
				}
			}

			// Processing a non-empty panel with no empties beneath it; check if it needs to land
			else if (grid[i][j]->state == Panel::Hovering || grid[i][j]->state == Panel::Falling)
			{
				// Check if panels below are ground
				if (j < grid_size.y - 1 ? grid[i][j + 1]->state != Panel::Hovering && grid[i][j + 1]->state != Panel::Falling : true)
				{
					UpdatePanelPosition(grid[i][j]->grid_pos.x, grid[i][j]->grid_pos.y);
					grid[i][j]->SetState(Panel::Idle); // Become one with the ground
					grid[i][j]->hover_time = 0;
					panels_to_check_for_clears.insert(grid[i][j]);

					panel_landed = true;
				}	
			}
		}
	}
}


void GameGrid::ProcessClears()
{
	std::unordered_set<Panel*> horizontal_matches;
	std::unordered_set<Panel*> vertical_matches;

	ClearGroup clear_group = ClearGroup(); // Discarded if no clears this frame

	// Check for new clears this frame
	for (auto panel : panels_to_check_for_clears)
	{	
#ifndef NDEBUG
		// Debug - should not happen
		if (panel->panel_type == Panel::Empty)
			std::cout << "no pls" << std::endl;
#endif
		// Filter out panels that were just swapping and forced done for a new swap that also contains this panel
		// You know... lets you skip through a clear if you're faster than the 4 swap frames
		if (panel->state == Panel::Swapping)
			continue; 
		
		horizontal_matches.clear();
		vertical_matches.clear();

		Panel::PanelType type = panel->panel_type;

		// Check for a horizontal clear	
		int x = panel->grid_pos.x;
		while (--x >= 0)
		{
			if (grid[x][panel->grid_pos.y]->panel_type != type || !grid[x][panel->grid_pos.y]->CanClear())
				break;

			horizontal_matches.insert(grid[x][panel->grid_pos.y]);
		}

		x = panel->grid_pos.x;
		while (++x < grid_size.x)
		{
			if (grid[x][panel->grid_pos.y]->panel_type != type || !grid[x][panel->grid_pos.y]->CanClear())
				break;

			horizontal_matches.insert(grid[x][panel->grid_pos.y]);
		}

		// Check for a vertical clear
		int y = panel->grid_pos.y;
		while (--y >= 0)
		{
			if (grid[panel->grid_pos.x][y]->panel_type != type || !grid[panel->grid_pos.x][y]->CanClear())
			{
				break;
			}

			vertical_matches.insert(grid[panel->grid_pos.x][y]);
		}

		y = panel->grid_pos.y;
		while (++y < grid_size.y)
		{
			if (grid[panel->grid_pos.x][y]->panel_type != type || !grid[panel->grid_pos.x][y]->CanClear())
			{
				break;
			}

			vertical_matches.insert(grid[panel->grid_pos.x][y]);
		}

		if (horizontal_matches.size() > 1)
		{
			for (auto panel : horizontal_matches)
				clear_group.panels.insert(panel);

			clear_group.panels.insert(panel);
		}
		if (vertical_matches.size() > 1)
		{
			for (auto panel : vertical_matches)
				clear_group.panels.insert(panel);

			clear_group.panels.insert(panel);
		}
	}

	if (clear_group.panels.size() > 2)
	{
		ClearColony::iterator it = clears.insert(clear_group);

		// Getting a clear cancels forced-rise
		force_rise = false;

		// Check for combo
		bool combo = false;
		if ((*it).panels.size() > 3)
		{
			ComboGet((*(*it).panels.begin())->position, (*it).panels.size());
			combo = true;
			combo_or_chain = true;
		}

		// Detect chain
		bool chain = false;
		for (auto panel : (*it).panels)
		{
			panel->SetState(Panel::Flashing);

			if (panel->can_chain)
			{
				chain = true;
			}
		}

		if (chain)
		{
			combo_or_chain = true;

			// If this chain was also a combo, raise the chain sign above the combo sign
			if (combo)
				ChainGet((*(*it).panels.begin())->position - Vector2f(0.f, Panel::SIZE));
			else
				ChainGet((*(*it).panels.begin())->position);

			// If this chain is greater than x2, accumulate pop count
			if (active_chain_group != nullptr)
				it->pop_count = active_chain_group->pop_count;
			else
				it->pop_count = 3;

			active_chain_group = &(*it);
		}

		score += score_chain[std::min(chain_count, max_chain)];
		score += score_combo[std::min(clear_group.panels.size() - 1, max_combo)];


		score_text->text = "SCORE:\n" + std::to_string(score);
	}

	// Finished processing panels for clears, so reset states and chainable flags
	for (auto panel : panels_to_check_for_clears)
	{
		// Panels that are to continue falling later will not
		bool skip = false;
		for (int y = panel->grid_pos.y; y < grid_size.y - 1; y++)
			if (grid[panel->grid_pos.x][y]->panel_type == Panel::Empty)
			{
				skip = true;
				break;
			}

		if (skip)
			continue;	

		if (panel->state != Panel::Flashing)
			panel->SetState(Panel::Idle);

		panel->can_chain = false;
	}

	panels_to_check_for_clears.clear();

	// Process cleargroups until complete
	ClearColony::iterator it = clears.begin();
	while (it != clears.end())
	{
		if ((*it).clear_time == time_flash[difficulty])
		{
			for (auto panel : (*it).panels)
				panel->SetState(Panel::Clearing);
		}
		// Pop panels in sequence
		else if ((*it).clear_time >= time_flash[difficulty] + time_match[difficulty]
			&& (*it).clear_time <= time_flash[difficulty] + time_match[difficulty] + (time_pop[difficulty] * (*it).panels.size() - 1))
		{
			if (((*it).clear_time - (time_flash[difficulty] + time_match[difficulty])) % time_pop[difficulty] == 0)
			{
				int pop_time = (*it).clear_time - (time_flash[difficulty] + time_match[difficulty]);
				
				int panel_index = pop_time / time_pop[difficulty];
				std::set<Panel*>::iterator it_panel = (*it).panels.begin();
				std::advance(it_panel, panel_index);

				(*it_panel)->Pop();
				audio_manager.PlaySFX(audio_manager.sfx_pop_map[std::min(9u, it->pop_count)]);
				++(it->pop_count);

				// 10 points per pop
				score += 10;
				score_text->text = "SCORE:\n" + std::to_string(score); // Yes, this is inefficient, but I don't care

				// Check if reached next speed level
				if (++clear_count == panels_to_next_speed[speed_level])
				{
					unsigned int tens = (speed_level + 1) / 10;
					if (tens > 0)
						speed_level_text->text.pop_back();

					speed_level_text->text.pop_back();

					clear_count = 0;
					++speed_level;
					
					tens = (speed_level + 1) / 10;
					if (tens > 0)
						speed_level_text->text.push_back('0' + tens);

					speed_level_text->text.push_back('0' + (speed_level + 1) % 10);
				}

				std::set<Panel*>::iterator check_end = it_panel;
				std::advance(check_end, 1);

				// Empty popped panel slots and set above panels for chaining
				if (check_end == (*it).panels.end())
				{
					std::vector<unsigned int> x_coords; 

					for (auto panel : (*it).panels)
					{
						int x = panel->grid_pos.x;
						int y = panel->grid_pos.y;

						// Each column needs to be processed only once
						// NOTE: this works because panel sorting order is top-down, left-right - otherwise it wouldn't
						if (std::find(x_coords.begin(), x_coords.end(), x) != x_coords.end())
						{
							// skip the rest of this iteration EXCEPT removal at end
							RemovePanel(x, y); 
							continue;
						}
						else
						{
							x_coords.push_back(x);
						}

						while (--y < 0 ? false : grid[x][y]->CanChainFall())
						{
							if (grid[x][y]->state == Panel::Swapping)
							{
								FinishSwap();

								// If an empty panel has been swapped in, skip to next column
								if (grid[x][y]->panel_type == Panel::Empty)
									break;
							}

							grid[x][y]->can_chain = true;
							grid[x][y]->state = Panel::Hovering;
							(*it).chainable_panels.push_back(grid[x][y]);
						}

						RemovePanel(x, panel->grid_pos.y);
					}
				}
			}
		}

		// Remove finished clears
		if (++(*it).clear_time > time_flash[difficulty] + time_match[difficulty] + (time_pop[difficulty] * (*it).panels.size() - 1))
		{
			(*it).chainable_panels.erase(std::remove_if((*it).chainable_panels.begin(), (*it).chainable_panels.end(), [](Panel* panel) {return panel->state != Panel::Hovering && panel->state != Panel::Falling && panel->state != Panel::Swapping; }), (*it).chainable_panels.end());

			// All panels that fell as a result of this clear have landed. If they chained, they would become the active clear group.
			// If the active chain group is still this group, that means a chain was not achieved, and the chain counter must be reset.
			if ((*it).chainable_panels.size() == 0)
			{
				if (active_chain_group == &(*it))
				{
					// Fanfare
					if (chain_count > 6)
						audio_manager.PlaySFX(AudioManager::FANFARE3);
					else if (chain_count > 4)
						audio_manager.PlaySFX(AudioManager::FANFARE2);
					else if (chain_count > 2)
						audio_manager.PlaySFX(AudioManager::FANFARE1);

					active_chain_group = nullptr;
					chain_count = 0;
				}

				it = clears.erase(it);
			}
			else
				++it;
		}
		else
			++it;
	}
}


void GameGrid::ChainGet(Vector2f pos)
{
	++chain_count;

	// Stop timer
	if (danger)
	{
		stop_this_frame = true;
		Stop(60 * stop_time_danger[difficulty]);
	}
	else
	{
		Stop(60 * (stop_timer < stop_time_chain[difficulty] ? stop_time_chain[difficulty] : 2));
	}

	// Spiral
	for (int i = 0; i < 6; i++)
	{
		// Particles destroy themselves; no need to maintain reference
		LogSpiralParticle* particle = LogSpiralParticle::GetFromPool();
		particle->Set(pos + Vector2f(11, 11), i * 60.f);
	}

	ChainSign* sign = ChainSign::GetFromPool(); // Recycles itself on a timer so losing scope without calling recycle is fine
	sign->Set(pos, chain_count + 1);
}

void GameGrid::ComboGet(Vector2f pos, unsigned int combo_count)
{
	// Stop timer
	if (danger)
	{
		stop_this_frame = true;
		Stop(60 * stop_time_danger[difficulty]);
	}
	else
	{
		Stop(60 * (stop_timer < stop_time_combo[difficulty] ? stop_time_combo[difficulty] : 2));
	}

	// Spiral
	for (int i = 0; i < 6; i++)
	{
		// Particles destroy themselves; no need to maintain reference
		LogSpiralParticle* particle = LogSpiralParticle::GetFromPool();
		particle->Set(pos + Vector2f(11, 11), i * 60.f);
	}

	ComboSign* sign = ComboSign::GetFromPool(); // Recycles itself on a timer so losing scope without calling recycle is fine
	sign->Set(pos, combo_count);
}

void GameGrid::Stop(unsigned int num_frames)
{
	speed_level_text->draw = false;

	stop_timer = std::min(600u, stop_timer + num_frames);
	stop_text->draw = true;
	stop_text->text = "STOP " + std::to_string((int)ceil(stop_timer / 60));
}

void GameGrid::CancelStop()
{
	stop_timer = 0;
	stop_text->draw = false;
	speed_level_text->draw = true;
}

void GameGrid::TickStop()
{
	// Stop time does not decrease while there are clears
	if (!clears.empty())
		return;

	if (stop_timer % 60 == 0)
		stop_text->text = "STOP " + std::to_string((int)ceil(stop_timer / 60));

	if (--stop_timer == 0)
	{
		stop_text->draw = false;
		speed_level_text->draw = true;
	}
}


void GameGrid::RaiseGrid()
{
	// If there is an active swap its position root needs to be raised
	--curr_swap.swap_root.y;

	// Deviation from classic games - swapper maintaining grid position is annoying (effectively jumps down on level rise)
	swapper->grid_pos.y = std::max(0, swapper->grid_pos.y - 1);

	for (int i = 0; i < grid_size.x; i++)
	{
		// If any columns are full, the game has been lost (this should never happen tho)
		if (grid[i][0]->panel_type != Panel::Empty)
			GameOver();

		// Shift columns up one space
		for (int j = 0; j < grid_size.y - 1; j++)
		{
			// Swap pointers
			Panel* temp = grid[i][j];
			grid[i][j] = grid[i][j + 1];
			grid[i][j + 1] = temp;
		}

		// Delete empty panel shifted from top
		grid[i][grid_size.y - 1]->Recycle();

		// Merge next_row into actual grid TODO:
		next_row[i]->color = { 0xFF, 0xFF, 0xFF, 0xFF };
		next_row[i]->size.y = Panel::SIZE;
		grid[i][grid_size.y - 1] = next_row[i];
		next_row[i] = nullptr;
		
		// Check for clears with new row
		panels_to_check_for_clears.insert(grid[i][grid_size.y - 1]);
	}

	GenerateNextRow();
	UpdatePanelPositions();
}


void GameGrid::GameOver()
{
	ChangeState(State::GAME_OVER);
}


void GameGrid::PausePanels()
{
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = 0; j < grid_size.y; j++)
		{
			if (grid[i][j]->panel_type == Panel::Empty)
				continue;

			grid[i][j]->SetActive(false, grid[i][j]->draw);
		}
	}
}


void GameGrid::UnpausePanels()
{
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = 0; j < grid_size.y; j++)
		{
			if (grid[i][j]->panel_type == Panel::Empty)
				continue;

			grid[i][j]->SetActive(true, grid[i][j]->draw);
		}
	}
}


void GameGrid::UpdatePanelPositions()
{
	// Raise panels
	for (int i = 0; i < grid_size.x; i++)
	{
		for (int j = 0; j < grid_size.y; j++)
		{
			// All panels rise
			grid[i][j]->position.y = grid_origin.y + j * Panel::SIZE - std::min((int)risen, Panel::SIZE) - bouncer.bounce;

			// Do not override the x-positions of swapping panels
			if (grid[i][j]->state != Panel::Swapping)
				grid[i][j]->position.x = grid_origin.x + i * Panel::SIZE;

			// Overwrite grid_pos in case it changed
			grid[i][j]->grid_pos = Vector2i(i, j);
		}
	}

	UpdateSwapperPosition();

	// Position new row beneath the stack
	for (unsigned int i = 0; i < next_row.size(); i++)
	{
		next_row[i]->position.x = grid_origin.x + i * Panel::SIZE;
		next_row[i]->position.y = grid_origin.y + grid_size.y * Panel::SIZE - std::min((int)risen, Panel::SIZE);

		next_row[i]->src.h = std::min((int)risen, Panel::SIZE);
		next_row[i]->size.y = std::min((int)risen, Panel::SIZE);
	}
}


void GameGrid::UpdatePanelPosition(unsigned int x, unsigned int y)
{
	grid[x][y]->grid_pos = Vector2i(x, y);

	// All panels rise
	grid[x][y]->position.y = grid_origin.y + y * Panel::SIZE - risen - bouncer.bounce;

	// Do not override the x-positions of swapping panels
	if (grid[x][y]->state != Panel::Swapping)
		grid[x][y]->position.x = grid_origin.x + x * Panel::SIZE;
}

void GameGrid::UpdateSwapperPosition()
{
	// Raise swapper
	swapper->position = SWAPPER_OFFSET + grid_origin + Vector2f(swapper->grid_pos * Panel::SIZE);
	swapper->position.y -= risen;
}


Panel* GameGrid::GetPanelAtWorldPos(Vector2f pos)
{
	pos.y += risen;
	
	if (pos.x < grid_origin.x || pos.x > grid_origin.x + grid_pixel_size.x || pos.y < grid_origin.y || pos.y > grid_origin.y + grid_pixel_size.y)
		return nullptr;

	int x = pos.x - grid_origin.x;
	x /= Panel::SIZE;

	int y = pos.y - grid_origin.y;
	y /= Panel::SIZE;

	return grid[x][y];
}

