#ifndef _ISerializable_
#define _ISerializable_
#pragma once

#include <Utils\BinaryWriter.h>
#include <Utils\BinaryReader.h>

class ISerializable
{
public:
	virtual void Serialize(BinaryWriter &bw) = 0;
	virtual void Deserialize(BinaryReader &br) = 0;
};

#endif
