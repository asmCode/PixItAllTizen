#include "CriticalSectionFactory.h"
#include "CriticalSection.h"

ICriticalSection* CriticalSectionFactory::Create()
{
	return new CriticalSection();
}
