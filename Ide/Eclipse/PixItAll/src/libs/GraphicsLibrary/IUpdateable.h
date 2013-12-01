#pragma once

class IUpdateable
{
public:
	virtual ~IUpdateable() {}
	virtual void Update(float time, float seconds) = 0;
};
