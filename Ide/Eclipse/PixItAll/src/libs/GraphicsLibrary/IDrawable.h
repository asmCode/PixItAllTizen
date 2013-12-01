#pragma once

class IDrawable
{
public:
	virtual ~IDrawable() {}
	virtual void Draw(float time, float seconds) = 0;
};
