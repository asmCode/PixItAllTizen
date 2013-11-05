#pragma once

class IDrawable
{
public:
	virtual void Draw(float time, float seconds) = 0;
};
