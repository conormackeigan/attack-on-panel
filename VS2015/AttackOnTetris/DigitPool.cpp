#include "DigitPool.h"
#include "Digit.h"
#include "TextureManager.h"
#include "RenderSystem.h"

std::vector<std::unique_ptr<Digit>> DigitPool::pool[11];
std::vector<unsigned int> DigitPool::next_available[11];

void DigitPool::InitPool(unsigned int pool_size)
{
	for (int i = 0; i < pool_size; i++) { pool[0].push_back(std::make_unique<Digit>(TextureManager::DIGIT_0)); pool[0].back()->digit = 0; pool[0].back()->index = i; next_available[0].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[1].push_back(std::make_unique<Digit>(TextureManager::DIGIT_1)); pool[1].back()->digit = 1; pool[1].back()->index = i; next_available[1].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[2].push_back(std::make_unique<Digit>(TextureManager::DIGIT_2)); pool[2].back()->digit = 2; pool[2].back()->index = i; next_available[2].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[3].push_back(std::make_unique<Digit>(TextureManager::DIGIT_3)); pool[3].back()->digit = 3; pool[3].back()->index = i; next_available[3].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[4].push_back(std::make_unique<Digit>(TextureManager::DIGIT_4)); pool[4].back()->digit = 4; pool[4].back()->index = i; next_available[4].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[5].push_back(std::make_unique<Digit>(TextureManager::DIGIT_5)); pool[5].back()->digit = 5; pool[5].back()->index = i; next_available[5].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[6].push_back(std::make_unique<Digit>(TextureManager::DIGIT_6)); pool[6].back()->digit = 6; pool[6].back()->index = i; next_available[6].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[7].push_back(std::make_unique<Digit>(TextureManager::DIGIT_7)); pool[7].back()->digit = 7; pool[7].back()->index = i; next_available[7].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[8].push_back(std::make_unique<Digit>(TextureManager::DIGIT_8)); pool[8].back()->digit = 8; pool[8].back()->index = i; next_available[8].push_back(i); }
	for (int i = 0; i < pool_size; i++) { pool[9].push_back(std::make_unique<Digit>(TextureManager::DIGIT_9)); pool[9].back()->digit = 9; pool[9].back()->index = i; next_available[9].push_back(i); }

	// Didn't bother using an enum since this is 10/11 naturally enumerated... so try to remember that 10 is the magic number for the "x" digit, ok?
	for (int i = 0; i < pool_size; i++) { pool[10].push_back(std::make_unique<Digit>(TextureManager::DIGIT_X)); pool[10].back()->digit = 10; pool[10].back()->index = i; next_available[10].push_back(i); }
}

Digit* DigitPool::GetFromPool(unsigned int digit)
{
	Digit* sprite = pool[digit][next_available[digit].back()].get();
	next_available[digit].pop_back();
	RenderSystem::Instance().RegisterRenderable(sprite);

	return sprite;
}

void DigitPool::RecycleDigit(Digit* digit_sprite)
{
	RenderSystem::Instance().UnregisterRenderable(digit_sprite);
	next_available[digit_sprite->digit].push_back(digit_sprite->index);
}
