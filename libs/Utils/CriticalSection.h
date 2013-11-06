#pragma once

#include "ICriticalSection.h"
#include "FBase.h"

class CriticalSection : public ICriticalSection
{
private:
	mutable Tizen::Base::Runtime::Monitor *m_lock;
	
public:
	CriticalSection();
	virtual ~CriticalSection() {}
	
	void Lock() const;
	void Unlock() const;
};
