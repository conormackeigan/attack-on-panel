Sample gameplay: https://www.youtube.com/watch?v=O9snMA95sIQ

Tetris Attack clone project, written in C++ with SDL2

Author: Conor MacKeigan, 2017

Controls:

Game Controller (recommended):

	Movement      : D-pad 

	Confirm/Swap  : A

	Cancel/Swap   : B

	Force Rise    : L/R Shoulder

	Pause/Confirm : Start

Keyboard:

	Movement      : Arrow Keys

	Confirm/Swap  : Z / Space

	Cancel/Swap   : X

	Force Rise    : A/S

	Pause/Confirm : Enter


Deviations from original:

-Empty panels beneath a swapping panel are swappable

-Some single-frame chains now have a 2 frame window

-A chainable panel will remain chainable until entire column below becomes grounded

-More responsive input (reduced latency)