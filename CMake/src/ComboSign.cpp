#include "ComboSign.h"
#include "DigitPool.h"
#include "Digit.h"

unsigned int ComboSign::rise_table[41] = { 0, 2, 4, 6, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 20, 20, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24 };

ComboSign::ComboSign() : IPooled<ComboSign>(TextureManager::TEXTURE_NAME::COMBO_SIGN)
{
	drawOrder = 1002;
}

void ComboSign::Set(Vector2f pos, unsigned int value)
{
	// Starting position
	position = pos;
	starting_height = pos.y;

	// Get digit sprites
	digits.clear();

	if (value > 99)
		value = 99;

	int tens = value / 10;
	if (tens > 0)
	{
		digits.push_back(DigitPool::GetFromPool(tens));
		digits.push_back(DigitPool::GetFromPool(value % 10));
	}
	else
	{
		digits.push_back(DigitPool::GetFromPool(value));
	}

	// Set digit x-positions
	float total_width = 0;
	for (auto digit : digits)
		total_width += digit->size.x;
	
	float last_x = (size.x - total_width) * 0.5f;
	for (auto digit : digits)
	{
		digit->position.x = position.x + last_x;
		digit->position.y = position.y;

		last_x += digit->size.x;
	}
}

void ComboSign::Update()
{
	// Rise (hard-coded table instead of exponential decay)
	position.y = starting_height - rise_table[timer];
	for (auto digit : digits)
		digit->position.y = position.y;

	// Automatically recycle to pool on timer
	if (++timer > 40)
		Recycle();
}

void ComboSign::Reset()
{
	timer = 0;
}

void ComboSign::Recycle()
{
	for (auto digit : digits)
		DigitPool::RecycleDigit(digit);

	IPooled::Recycle();
}
