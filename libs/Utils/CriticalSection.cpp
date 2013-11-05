#include "CriticalSection.h"


CriticalSection::CriticalSection()
{
	nsLock = [[NSRecursiveLock alloc] init];
}

void CriticalSection::Lock() const
{
	[nsLock lock];
}

void CriticalSection::Unlock() const
{
	[nsLock unlock];
}