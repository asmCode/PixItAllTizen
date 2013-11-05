#pragma once

#include <Graphics/IUpdateable.h>
#include <Graphics/IDrawable.h>
#include "IGestureHandler.h"

class Control;

class IGameState :
	public IUpdateable,
	public IDrawable,
	public IGestureHandler

{
public:
	virtual ~IGameState() {}
	virtual bool Initialize() = 0;
	
	virtual Control *GetMainPanel() = 0;
	virtual void SetFocus() = 0;
};
