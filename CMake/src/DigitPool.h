#pragma once

#include <vector>
#include <memory>

class Digit;

class DigitPool
{
public:
	DigitPool() = default;
	~DigitPool() = default;

	static void InitPool(unsigned int pool_size);
	static Digit* GetFromPool(unsigned int digit);
	static void RecycleDigit(Digit* digit_sprite);

private:
	static std::vector<std::unique_ptr<Digit>> pool[11];

public:
	static std::vector<unsigned int> next_available[11];
};

