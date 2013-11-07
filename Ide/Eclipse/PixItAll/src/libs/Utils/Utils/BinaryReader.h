#ifndef BINARY_READER_H
#define BINARY_READER_H

#include <string>
#include <memory.h>

class BinaryReader
{
private:
	void *m_pMem;
	
public:
	BinaryReader(void *pMem)
	{
		this->m_pMem = pMem;
	}
	
	template <typename T>
	T Read()
	{
		T val;
		memcpy(&val, m_pMem, sizeof(T));
		m_pMem = (T*)m_pMem + 1;
		return val;
	}
	
	void ReadBuffer(char *pBuff, unsigned size)
	{
		memcpy(pBuff, m_pMem, size);
		m_pMem = (char*)m_pMem + size;
	}
};

template <>
std::string BinaryReader::Read<std::string>();

#endif // BINARY_READER_H
