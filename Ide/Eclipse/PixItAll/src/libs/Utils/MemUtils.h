#pragma once

class MemUtils
{
public:
	template <typename T>
	static void DeleteObject(T &obj)
	{
		if (obj != 0)
		{
			delete obj;
			obj = 0;
		}
	}
	
	template <typename T>
	static void DeleteVector(T &obj)
	{
		if (obj != 0)
		{
			delete [] obj;
			obj = 0;
		}
	}
};
