#pragma once

class ICriticalSection
{
public:
	virtual ~ICriticalSection() {}

	virtual void Lock() const = 0;
	virtual void Unlock() const = 0;
};
