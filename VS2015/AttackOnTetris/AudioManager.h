#pragma once

extern "C" {
#include <SDL.h>
#include "SDL_mixer.h"
}

class AudioManager
{
public:
	static AudioManager& Instance();

	AudioManager(const AudioManager&) = delete;
	void operator=(const AudioManager&) = delete;

private:
	AudioManager() = default;

public:
	void Init();
	void Shutdown();

	enum SFX
	{
		MOVEMENT,

		MENU_VALIDATE,
		MENU_CANCEL,

		//COUNTDOWN,
		//GO,

		SWAP,

		POP1,
		POP2,
		POP3,
		POP4,
		POP5,
		POP6,
		POP7,
		POP8,
		POP9,
		POP10,

		PANEL_LAND,

		COMBO_CHAIN_GET,

		FANFARE1,
		FANFARE2,
		FANFARE3,

		STOP,

		NUM_SFX
	};

	enum MUSIC
	{
		WATER_STAGE,
		WATER_DANGER,

		NUM_MUSIC
	};

	void PlaySFX(SFX sfx);
	void PlayMusic(MUSIC music, int ms_fadeout, int ms_fadein);
	void PauseMusic();
	void ResumeMusic();
	void StopMusic(int ms_fadeout);

	void ToggleMute();

private:
	int volume;

	// SFX
	Mix_Chunk* sfx_map[NUM_SFX];

public:
	SFX sfx_pop_map[10];

private:
	Mix_Chunk* sfx_movement = nullptr;
	Mix_Chunk* sfx_menu_validate = nullptr;
	Mix_Chunk* sfx_menu_cancel = nullptr;
	Mix_Chunk* sfx_countdown = nullptr;
	Mix_Chunk* sfx_go = nullptr;
	Mix_Chunk* sfx_swap = nullptr;
	Mix_Chunk* sfx_pop1 = nullptr;
	Mix_Chunk* sfx_pop2 = nullptr;
	Mix_Chunk* sfx_pop3 = nullptr;
	Mix_Chunk* sfx_pop4 = nullptr;
	Mix_Chunk* sfx_pop5 = nullptr;
	Mix_Chunk* sfx_pop6 = nullptr;
	Mix_Chunk* sfx_pop7 = nullptr;
	Mix_Chunk* sfx_pop8 = nullptr;
	Mix_Chunk* sfx_pop9 = nullptr;
	Mix_Chunk* sfx_pop10 = nullptr;
	Mix_Chunk* sfx_panel_land = nullptr;
	Mix_Chunk* sfx_combo_chain_get = nullptr;
	Mix_Chunk* sfx_fanfare1 = nullptr;
	Mix_Chunk* sfx_fanfare2 = nullptr;
	Mix_Chunk* sfx_fanfare3 = nullptr;
	Mix_Chunk* sfx_stop = nullptr;

	// Music
	Mix_Music* bgm_map[NUM_MUSIC];

	Mix_Music* bgm_water_stage = nullptr;
	Mix_Music* bgm_water_danger = nullptr;
};

