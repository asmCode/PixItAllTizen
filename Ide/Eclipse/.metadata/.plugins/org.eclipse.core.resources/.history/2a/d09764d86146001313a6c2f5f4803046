#pragma once

#include <fstream>
#include <vector>
#include "../Utils/Utils/BinaryReader.h"
#include "../Graphics/Animation.h"
#include "IAssetLoader.h"

class AnimationLoader : public IAssetLoader
{
private:
	Animation *LoadAnimation(BinaryReader &br);
	
	IInterpolator<sm::Vec3>* LoadVec3Anim(BinaryReader &br, sm::Vec3 &localVec3);
	IInterpolator<sm::Quat>* LoadQuatAnim(BinaryReader &br, sm::Quat &localQuat);
	sm::Matrix LoadMatrix(BinaryReader &br);
	
public:
	~AnimationLoader();
	void *Load(const char *path);
};
