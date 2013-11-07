#include "CriticalSection.h"

using namespace Tizen::Base::Runtime;

CriticalSection::CriticalSection()
{
	m_lock = new Monitor();
	m_lock->Construct();
}

void CriticalSection::Lock() const
{
	m_lock->Enter();
}

void CriticalSection::Unlock() const
{
	m_lock->Exit();
}
