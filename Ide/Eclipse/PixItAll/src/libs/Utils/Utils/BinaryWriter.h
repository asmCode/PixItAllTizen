#pragma once

#include <windows.h>
#include <ostream>

class BinaryWriter
{
private:
	std::ostream *os;

public:
	BinaryWriter(std::ostream *os)
	{
		this ->os = os;
	}

	void Write(BYTE val)
	{
		os ->write((char*)&val, sizeof(val));
	}

	void Write(char val)
	{
		os ->write((char*)&val, sizeof(val));
	}

	void Write(int val)
	{
		os ->write((char*)&val, sizeof(val));
	}

	void Write(float val)
	{
		os ->write((char*)&val, sizeof(float));
	}

	void Write(DWORD val)
	{
		os ->write((char*)&val, sizeof(val));
	}

	void Write(void *val, int size)
	{
		os ->write((char*)val, size);
	}

	void Write(const char *val)
	{
		int strLen = strlen(val);
		os ->write((char*)&strLen, sizeof(int));
		if (strLen > 0)
			os ->write(val, strLen);
	}
};
