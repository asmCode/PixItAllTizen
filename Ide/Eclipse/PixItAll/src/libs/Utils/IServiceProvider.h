#pragma once
#ifndef _ISERVICEPROVIDER_
#define _ISERVICEPROVIDER_

#undef IServiceProvider

namespace sm
{
	class IServiceProvider
	{
	public:
		virtual void *GetService(const char *typeName) = 0;
	};
}

#endif