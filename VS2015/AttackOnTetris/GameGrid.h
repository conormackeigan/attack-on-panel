#pragma once

#include "Entity.h"
#include "Settings.h"
#include "Panel.h"
#include "Vector2.h"
#include <vector>
#include <set>
#include <unordered_set>
#include "plf_colony.h"

class Swapper;
class GameTimer;
class UIText;
class PauseMenu;

#define BORDER_THICKNESS 10 // Pixel thickness of the grid border

class GameGrid : public Entity
{
public:
	GameGrid(GameMode game_mode, Vector2i grid_size, Difficulty difficulty);
	~GameGrid() = default;

	enum State
	{
		NONE,

		WAIT,
		FADEIN_PANELS,
		READY_DROP,
		COUNTDOWN,

		PLAY,
		GAME_OVER,
		PAUSE
	};

	void Init() override;
	void Update() override;
	void OnDestroy() override;

	void StartGame();

	void ChangeState(State new_state);

	// Returns position of top-left corner of grid
	Vector2f GetGridOrigin();

	// Get pixel size of the grid
	Vector2f GetPixelSize();

private:
	Swapper* swapper = nullptr; // The player
	static const Vector2f SWAPPER_OFFSET;

	State state = NONE;
	unsigned int state_time = 0; 

	//------------------------------------------------
	// Grid Data
	//------------------------------------------------
	std::vector<std::vector<Panel*>> grid;

	std::vector<Panel*> next_row; // Next row of panels, rising from the bottom
	
	// Game parameters
	GameMode game_mode;
	Vector2i grid_size;
	Difficulty difficulty;

	// Commonly accessed calculations
	Vector2i grid_pixel_size;
	Vector2f grid_origin; // Point in space of the top-left corner of the top-left panel slot
	
	Sprite* border[4]; // Four solid rectangles for the border

	//--------------------------------------------------
	// Initialization
	//--------------------------------------------------
	bool unlock_swapper = false; // Unlock swapper when it reaches starting position

	void Generate(); // Generate initial grid
	
	// Drop a random panel type into the specified column
	void DropInRandPanel(unsigned int x);

	// Returns true if putting a panel at x, y results in a clear (does not check above)
	bool CheckSpawnClear(unsigned int x, unsigned int y, Panel::PanelType panel_type);

	//--------------------------------------------------
	// Grid functions
	//--------------------------------------------------
	// Adding to / removing from grid (updates vector of active panels)
	void AddPanel(Panel::PanelType panel_type, unsigned int x, unsigned int y);
	void RemovePanel(unsigned int x, unsigned int y);
	void RemovePanel(Panel* panel);

	void GenerateNextRow(); // Generate new row to rise up from bottom

	void RaiseGrid(); // Raise the grid up one row, adding next_row to the bottom of grid[][]
	void UpdatePanelPositions(); // Update positions of everything to their gridpos - risen value
	void UpdatePanelPosition(unsigned int x, unsigned int y);
	void UpdateSwapperPosition();

	void SwapperInput();
	void StartSwap();
	void TickSwap();
	void FinishSwap();

	void ApplyGravity(); // Iterate all panels and update their hanging/falling state if necessary

	void ProcessClears(); // Check if there were any clears this frame

	void ChainGet(Vector2f pos);
	void ComboGet(Vector2f pos, unsigned int combo_count);

	void Stop(unsigned int num_frames);
	void CancelStop();
	void TickStop();

	void GameOver();

	//----------------------------------------------------
	// Grid State
	//----------------------------------------------------
	struct PanelBouncer
	{
		// bounce levels indexed by state time
		int bounce_table[39] = { 0, 16, 16, 8, 0, 8, 0, 6, 0, 6, 0, 4, 0, 4, 0, 4, 0, 4, 0, 3, 0, 3, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 2, 0, 1, 0, 1, 0 };
		int bounce = 0; // Current pixel bounce level
		int state_time = 39;

		void StartBounce()
		{
			state_time = std::max(0, state_time - 39); // Support for accumulative bounces (not implemented yet, needs a bigger table)
		}

		void ForceBounceEnd()
		{
			bounce = 0;
			state_time = 39;
		}

		void Update()
		{
			if (state_time < 39)
			{
				bounce = bounce_table[state_time];
				++state_time;
			}
		}
	};
	
	PanelBouncer bouncer;

	Vector2i swapper_target; // For the swapper moving to center of grid on init

	// SFX flags
	bool panel_landed = false;
	bool combo_or_chain = false;
	bool stop_this_frame = false;
	
	unsigned int risen; // Number of pixels risen since start of row (height = 32)
	unsigned int rise_timer;

	unsigned int stop_timer = 0;

	unsigned long score = 0;
	unsigned int speed_level = 0;
	unsigned int clear_count = 0; // Smaller data type than uint32 guarantees it will never hit level >99
	unsigned int chain_count = 0;

	int max_health; // Max health for the game mode (1 in endless/time attack)
	int health;
	bool damage = false;
	bool danger = false;
	bool force_rise = false; // Activated by shoulder buttons to quickly raise grid
	
	// Swapping
	struct Swap
	{
		Panel* left = nullptr;
		Panel* right = nullptr;

		uint8_t curr_time = 5; // How far into current swap (> swap_time means no active swap)
		uint8_t swap_time = 4; // Total time required for a swap

		Vector2i swap_root; // The position of the swapper when the swap started
	};

	Swap curr_swap;

	// Keep track of all updated panels and check if they result in a clear
	std::unordered_set<Panel*> panels_to_check_for_clears;

	struct ComparePanel
	{
		bool operator()(Panel* lhs, Panel* rhs)
		{
			return *lhs < *rhs;
		}
	};

	// A group of panels cleared on the same frame
	// Will remain alive until all panels that fall as a result are processed for clears
	struct ClearGroup
	{
		ClearGroup()
		{
			panels = std::set<Panel*, ComparePanel>();
		}

		std::set<Panel*, ComparePanel> panels; // Needs to be sorted for pop order
		std::vector<Panel*> chainable_panels; // Chainable panels associated with this clear

		unsigned int clear_time = 0;
		unsigned int pop_count = 0;

		bool operator==(const ClearGroup& rhs) { return this == &rhs; }
	};

	typedef plf::colony<ClearGroup> ClearColony; // THANK YOU BASED COLONY FOR MAINTAINING POINTER/INDEX VALIDITY
	ClearColony clears; // Confirmed clears to be flashed/popped
	ClearGroup* active_chain_group = nullptr; // The clear group the current chain (if any) is associated with

	// Transition
	int fade_value = 0x00;
	bool panels_dying = false;
	bool fading_out = false;
	int fade_out_row = 0;

	//----------------------------------------------------
	// UI
	//----------------------------------------------------
	GameTimer* game_timer = nullptr;
	UIText* score_text = nullptr;
	UIText* stop_text = nullptr;
	UIText* speed_level_text = nullptr;
	UIText* difficulty_text = nullptr;

	// Start game UI
	UIText* ready_text = nullptr;
	float ready_target_y;

	UIText* countdown_text = nullptr;
	uint8_t countdown_value;

public:
	PauseMenu* pause_menu = nullptr;
	bool unpausing = false; // Flag to control alpha fade

	void PausePanels();
	void UnpausePanels();

	//----------------------------------------------------
	// Helper functions
	//----------------------------------------------------
	Panel* GetPanelAtWorldPos(Vector2f pos); // Only used for debugging
};

