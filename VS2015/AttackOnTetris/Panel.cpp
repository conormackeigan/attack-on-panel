#pragma warning(disable:4554) 
#pragma warning(disable:4800)

#include "Panel.h"
#include "TextureManager.h"
#include "EntityManager.h"
#include "AnimatorPanel.h"
#include <sstream>
#include <iostream>

#define entity_manager EntityManager::Instance()

const uint8_t Panel::can_swap_mask  = 1 << Panel::Idle | 1 << Panel::Swapping | 1 << Panel::Falling;
const uint8_t Panel::can_fall_mask  = 1 << Panel::Idle | 1 << Panel::Hovering | 1 << Panel::Falling;
const uint8_t Panel::can_clear_mask = 1 << Panel::Idle | 1 << Panel::Falling;
const uint8_t Panel::can_chain_mask = can_fall_mask | 1 << Panel::Swapping;

const int Panel::SIZE = 32;

Panel::Panel() : IPooled(), hover_time(0)
{
	animator = std::make_unique<AnimatorPanel>(this);
	dead_sprite = std::make_unique<Sprite>();
	dead_sprite->color = { 0xFF, 0xFF, 0xFF, 0xFF };
	dead_sprite->size = { Panel::SIZE, Panel::SIZE };
}

Panel::~Panel()
{
}

void Panel::Swap(Panel& lhs, Panel& rhs)
{
	// Swap data when panels are swapped in-game to maintain pointer validity for chaingroups
	// Panels should never be animating when they're swapped, so no need to swap animators
	std::swap(lhs.active, rhs.active);
	std::swap(lhs.draw, rhs.draw);
	std::swap(lhs.size, rhs.size);
	std::swap(lhs.color, rhs.color);
	std::swap(lhs.drawOrder, rhs.drawOrder);
	std::swap(lhs.grid_pos, rhs.grid_pos);
	std::swap(lhs.panel_type, rhs.panel_type);
	std::swap(lhs.texture, rhs.texture);
	lhs.dead_sprite.swap(rhs.dead_sprite);
	std::swap(lhs.can_chain, rhs.can_chain);
}


void Panel::Init()
{
}

void Panel::Update()
{
}

void Panel::OnDestroy()
{
	IPooled<Panel>::OnDestroy();

	// Reset panel
	SetState(Idle);
	src.h  = SIZE;
	size.y = SIZE;
	position = Vector2f::zero;
}

void Panel::Render(SDL_Renderer* renderer)
{
	Sprite::Render(renderer);

	// If dead render deadmask cuz no shaders
	if (dead)
	{
		dead_sprite->position = position;
		dead_sprite->Render(renderer);
	}
}

Panel* Panel::GetFromPool(PanelType type)
{
	Panel* panel = IPooled::GetFromPool();

	panel->dead = false;
	panel->SetPanelType(type);

	if (type == Empty)
	{
		panel->draw = false;
	}
	else
	{
		panel->color = { 0xFF, 0xFF, 0xFF, 0xFF };
		panel->SetActive(true);
	}
	
	return panel;
}

void Panel::SetPanelType(PanelType panel_type)
{
	this->panel_type = panel_type;

	// Get a pointer to the new texture
	switch (panel_type)
	{
	case Empty:
		draw = false;
		break;

	case Blank:
		SetTexture(TextureManager::PANEL_BLANK);
		break;

	case Red:
		SetTexture(TextureManager::PANEL_RED);
		dead_sprite->SetTexture(TextureManager::PANEL_RED_DEAD);
		break;

	case Green:
		SetTexture(TextureManager::PANEL_GREEN);
		dead_sprite->SetTexture(TextureManager::PANEL_GREEN_DEAD);
		break;

	case Yellow:
		SetTexture(TextureManager::PANEL_YELLOW);
		dead_sprite->SetTexture(TextureManager::PANEL_YELLOW_DEAD);
		break;

	case Cyan:
		SetTexture(TextureManager::PANEL_CYAN);
		dead_sprite->SetTexture(TextureManager::PANEL_CYAN_BLUE_DEAD);
		break;

	case Purple:
		SetTexture(TextureManager::PANEL_PURPLE);
		dead_sprite->SetTexture(TextureManager::PANEL_PURPLE_DEAD);
		break;

	case Blue:
		SetTexture(TextureManager::PANEL_BLUE);
		dead_sprite->SetTexture(TextureManager::PANEL_CYAN_BLUE_DEAD);
		break;

	default: // DEBUG
		std::cerr << "Trying to set panel to a type not in Panel::SetPanelType switch statement" << std::endl;
		break;
	}
}

void Panel::SetState(State s)
{
	state = s;

	// Enter new state
	switch (state)
	{
	case Idle:
		animator->SetState("Idle");
		break;

	case Flashing:
		animator->SetState("Flashing");
		break;

	case Clearing:
		animator->SetState("Clearing");
		break;
	}
}

void Panel::Die()
{
	SetState(Panel::State::Clearing);
	dead = true;
	dead_sprite->color.a = 0x00;
}

bool Panel::CanSwap()
{	
	return 1 << state & can_swap_mask;
}

bool Panel::CanFall()
{
	return 1 << state & can_fall_mask;
}

bool Panel::CanClear()
{
	return 1 << state & can_clear_mask;
}

bool Panel::CanChainFall()
{
	return 1 << state & can_chain_mask && panel_type != Empty;
}

void Panel::Pop()
{
	// todo: particles and stuff
	draw = false;
}

bool operator<(const Panel& lhs, const Panel& rhs)
{
	// Sort panels top-bottom, if y is the same then left-right
	return lhs.position.y < rhs.position.y ? true : lhs.position.y > rhs.position.y ? false : lhs.position.x < rhs.position.x ? true : false;
}

std::string Panel::ToString()
{
	std::stringstream ss;
	ss << "------------------------------------------" << std::endl;
	ss << "Position: " << position.x << ", " << position.y << std::endl;
	ss << "Type: " << panel_type << std::endl;
	ss << "State: " << state << std::endl;
	ss << "Grid Pos: " << grid_pos.x << ", " << grid_pos.y << std::endl;
	ss << "Hover Time: " << hover_time << std::endl;
	ss << "Can Chain? : " << can_chain << std::endl;
	ss << "------------------------------------------" << std::endl << std::endl;

	return ss.str();
}