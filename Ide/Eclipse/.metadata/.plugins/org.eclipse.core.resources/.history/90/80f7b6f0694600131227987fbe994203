#pragma once

#include "IGameState.h"

class Game;

class BaseGameState : public IGameState
{
protected:
	Game *game;
	
public:
	BaseGameState(Game *game);
	virtual ~BaseGameState() {};
	
	virtual void SetFocus();
};
