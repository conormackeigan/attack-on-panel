#pragma once

#include "Entity.h"

class UIText;

// This is hard-coded as hell, just wanted something functional quick
class PauseMenu : public Entity
{
public:
	PauseMenu(int x, int y);
	~PauseMenu();

	enum Selection
	{
		CONTINUE,
		QUIT
	};

	Selection selection = CONTINUE;

	bool done = false;

	void Init() override;
	void Update() override;
	void OnDestroy() override;

private:
	int x, y; // Origin (center)

	UIText* pointer;
	UIText* text_continue;
	UIText* text_quit;
};

