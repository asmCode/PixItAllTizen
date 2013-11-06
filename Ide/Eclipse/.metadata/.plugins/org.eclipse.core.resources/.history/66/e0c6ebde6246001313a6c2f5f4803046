#pragma once

#include "ICriticalSection.h"

class CriticalSection : public ICriticalSection
{
private:
	mutable NSRecursiveLock *nsLock;
	
public:
	CriticalSection();
	
	void Lock() const;
	void Unlock() const;
};
