#include "BinaryReader.h"

template <>
std::string BinaryReader::Read<std::string>()
{
	std::string val;
	unsigned len = Read<unsigned>();
	if (len > 0)
	{
		char *str = new char[len];
		ReadBuffer(str, len);
		val.assign(str, len);
		delete [] str;
	}
	return val;
}
