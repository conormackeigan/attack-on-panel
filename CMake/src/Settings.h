#pragma once

#include <math.h>
#include <limits>

//------------------------------------
// Application parameters
//------------------------------------
static const int SCREEN_WIDTH = 720;
static const int SCREEN_HEIGHT = 480;

//--------------------------------------------------------------------------------------------
// Gameplay parameters 
// (modified versions of https://github.com/sharpobject/panel-attack/blob/master/globals.lua)
//--------------------------------------------------------------------------------------------

enum Difficulty
{
	EASY,
	NORMAL,
	HARD,
	VHARD
};

enum GameMode
{
	ENDLESS,
	TIME_ATTACK
};

// Score lookup tables
static const unsigned int score_combo[] =
{  0,    0,    0,   20,   30,
  50,   60,   70,   80,  100,
 140,  170,  210,  250,  290,
 340,  390,  440,  490,  550,
 610,  680,  750,  820,  900,
 980, 1060, 1150, 1240, 1330,
1420, 1510, 1600, 1700, 1800,
1900, 2000, 2150, 2300, 2450,
2600, 2750, 2900, 3100, 3300,
3500, 3700, 3900, 4100, 4400,
4700, 5000, 5300, 5600, 6000,
6500, 7000, 7500, 8000, 8500,
9000, 9600, 10200,10800,12000 };

static const unsigned int max_combo = 64; // index of max combo value

static const unsigned int score_chain[] =
{  0,   50,   80,  150,  300,
 400,  500,  700,  900, 1100,
1300, 1500, 1800, 2100, 2400,
2600, 2800, 3000, 3300, 3600,
4000, 4200, 4400, 4600, 4800,
5000, 5250, 5500, 5750, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000, 6000,
6000, 6000, 6000, 6000
};

static const unsigned int max_chain = 98;

// Action timings by difficulty (easy, normal, hard, v-hard)
static const unsigned int time_hover[]	= { 11,  8,  5,  2 };
static const unsigned int time_match[]	= { 25, 20, 15, 10 };
static const unsigned int time_flash[]  = { 44, 36, 28, 20 };
static const unsigned int time_pop[]	= {  9,  8,  7,  5 };

static const unsigned int stop_time_combo[]	 = {  2, 2, 2, 1 };
static const unsigned int stop_time_chain[]	 = {  5, 3, 2, 1 };
static const unsigned int stop_time_danger[] = { 10, 7, 4, 2 };

// Yes, 2 is slower than 1 and 50-99 are the same
static const unsigned int speed_to_rise_time[] =
{ 942, 983, 838, 790, 755, 695, 649, 604, 570, 515,
  474, 444, 394, 370, 347, 325, 306, 289, 271, 256,
  240, 227, 213, 201, 189, 178, 169, 158, 148, 138,
  129, 120, 112, 105,  99,  92,  86,  82,  77,  73,
   69,  66,  62,  59,  56,  54,  52,  50,  48,  47,
   47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
   47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
   47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
   47,  47,  47,  47,  47,  47,  47,  47,  47,  47,
   47,  47,  47,  47,  47,  47,  47,  47,  47 };

// speed_to_rise_time values are divided by these values corresponding to difficulty level
static const unsigned int speed_to_rise_difficulty_modifier[] = { 16, 32, 47, 60 };

// Number of cleared panels required to reach the next speed level (difficulty agnostic)
static const unsigned int panels_to_next_speed[] =
{ 9, 12, 12, 12, 12, 12, 15, 15, 18, 18,
 24, 24, 24, 24, 24, 24, 21, 18, 18, 18,
 36, 36, 36, 36, 36, 36, 36, 36, 36, 36,
 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, 45, 45,
 45, 45, 45, 45, 45, 45, 45, 45, std::numeric_limits<unsigned int>::infinity() };


