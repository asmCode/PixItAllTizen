#pragma once

#include "IKeysContainer.h"

template <typename Type>
class IInterpolator : public IKeysContainer
{
public:
	virtual ~IInterpolator() {}
	virtual void GetValue(float time, Type &value) = 0;
	virtual void AddKeyframe(float time, Type value, bool stopKey) = 0;
	virtual void GetKeyframe(int index, float &time, Type &value, bool &stopKey) = 0;
};
