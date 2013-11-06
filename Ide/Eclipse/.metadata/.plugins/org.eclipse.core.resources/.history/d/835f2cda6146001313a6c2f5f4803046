#pragma once
#ifndef _sb_
#define _sb_

#include <sstream>

// helper string builder class
// use it when you want to concat strings
// sb() + "error: " + code;
class sb
{
private:
	mutable std::stringstream ss;

public:
	template <typename T>
	sb& operator+(T src)
	{
		ss << src;

		return (*this);
	}

	operator std::string() const
	{
		ss.flush();
		return ss.str();
	}

	operator const std::string()
	{
		ss.flush();
		return ss.str();
	}
};

#endif
