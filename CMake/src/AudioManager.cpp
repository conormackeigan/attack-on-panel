#include "AudioManager.h"
#include <iostream>


AudioManager& AudioManager::Instance()
{
	static AudioManager instance;

	return instance;
}

void AudioManager::Init()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		std::cerr << "Error opening audio in AudioManager::Init" << std::endl;
	}

	volume = Mix_Volume(-1, -1);

	// SFX
	for (int i = 0; i < NUM_SFX; i++)
		sfx_map[i] = nullptr;

	sfx_movement = Mix_LoadWAV("audio/movement.flac");
	sfx_map[MOVEMENT] = sfx_movement;

	sfx_menu_validate = Mix_LoadWAV("audio/menu_validate.wav");
	sfx_map[MENU_VALIDATE] = sfx_menu_validate;

	sfx_menu_cancel = Mix_LoadWAV("audio/menu_cancel.wav");
	sfx_map[MENU_CANCEL] = sfx_menu_cancel;

	/*sfx_countdown = Mix_LoadWAV("audio/countdown.wav");
	sfx_map[COUNTDOWN] = sfx_countdown;

	sfx_go = Mix_LoadWAV("audio/go.wav");
	sfx_map[GO] = sfx_go;*/

	sfx_swap = Mix_LoadWAV("audio/swap.flac");
	sfx_map[SWAP] = sfx_swap;

	sfx_pop1 = Mix_LoadWAV("audio/pop1.flac");
	sfx_map[POP1] = sfx_pop1;
	sfx_pop_map[0] = POP1;

	sfx_pop2 = Mix_LoadWAV("audio/pop2.flac");
	sfx_map[POP2] = sfx_pop2;
	sfx_pop_map[1] = POP2;

	sfx_pop3 = Mix_LoadWAV("audio/pop3.flac");
	sfx_map[POP3] = sfx_pop3;
	sfx_pop_map[2] = POP3;

	sfx_pop4 = Mix_LoadWAV("audio/pop4.flac");
	sfx_map[POP4] = sfx_pop4;
	sfx_pop_map[3] = POP4;

	sfx_pop5 = Mix_LoadWAV("audio/pop5.flac");
	sfx_map[POP5] = sfx_pop5;
	sfx_pop_map[4] = POP5;

	sfx_pop6 = Mix_LoadWAV("audio/pop6.flac");
	sfx_map[POP6] = sfx_pop6;
	sfx_pop_map[5] = POP6;

	sfx_pop7 = Mix_LoadWAV("audio/pop7.flac");
	sfx_map[POP7] = sfx_pop7;
	sfx_pop_map[6] = POP7;

	sfx_pop8 = Mix_LoadWAV("audio/pop8.flac");
	sfx_map[POP8] = sfx_pop8;
	sfx_pop_map[7] = POP8;

	sfx_pop9 = Mix_LoadWAV("audio/pop9.flac");
	sfx_map[POP9] = sfx_pop9;
	sfx_pop_map[8] = POP9;

	sfx_pop10 = Mix_LoadWAV("audio/pop10.flac");
	sfx_map[POP10] = sfx_pop10;
	sfx_pop_map[9] = POP10;

	sfx_panel_land = Mix_LoadWAV("audio/panel_land.flac");
	sfx_map[PANEL_LAND] = sfx_panel_land;

	sfx_combo_chain_get = Mix_LoadWAV("audio/combo_chain_get.ogg");
	sfx_map[COMBO_CHAIN_GET] = sfx_combo_chain_get;

	sfx_fanfare1 = Mix_LoadWAV("audio/fanfare1.ogg");
	sfx_map[FANFARE1] = sfx_fanfare1;

	sfx_fanfare2 = Mix_LoadWAV("audio/fanfare2.ogg");
	sfx_map[FANFARE2] = sfx_fanfare2;

	sfx_fanfare3 = Mix_LoadWAV("audio/fanfare3.ogg");
	sfx_map[FANFARE3] = sfx_fanfare3;

	sfx_stop = Mix_LoadWAV("audio/stop.wav");
	sfx_map[STOP] = sfx_stop;

	// Music
	for (int i = 0; i < NUM_MUSIC; i++)
		bgm_map[i] = nullptr;

	bgm_water_stage = Mix_LoadMUS("audio/water_stage.ogg");
	bgm_map[WATER_STAGE] = bgm_water_stage;

	bgm_water_danger = Mix_LoadMUS("audio/water_danger.ogg");
	bgm_map[WATER_DANGER] = bgm_water_danger;
}

void AudioManager::Shutdown()
{
	// Free SFX
	for (int i = 0; i < NUM_SFX; i++)
		Mix_FreeChunk(sfx_map[i]);

	// Free Music
	for (int i = 0; i < NUM_MUSIC; i++)
		Mix_FreeMusic(bgm_map[i]);

	Mix_CloseAudio();
}

void AudioManager::PlaySFX(SFX sfx)
{
	Mix_PlayChannel(-1, sfx_map[sfx], 0);
}

void AudioManager::PlayMusic(MUSIC music, int ms_fadeout, int ms_fadein)
{
	Mix_FadeOutMusic(ms_fadeout);

	if (Mix_FadeInMusic(bgm_map[music], -1, ms_fadein) == -1)
		std::cerr << "Error playing music: " << Mix_GetError() << std::endl;
}

void AudioManager::PauseMusic()
{
	Mix_PauseMusic();
}

void AudioManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::StopMusic(int ms_fadeout)
{
	Mix_FadeOutMusic(ms_fadeout);
}

void AudioManager::ToggleMute()
{
	if (Mix_Volume(-1, -1) > 0)
	{
		Mix_Volume(-1, 0);
	}
	else
	{
		Mix_Volume(-1, volume);
	}
}

