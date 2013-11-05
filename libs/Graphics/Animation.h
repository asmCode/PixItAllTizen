#pragma once

#include <vector>
#include "../Utils/Math/Vec3.h"
#include "../Utils/Math/Quat.h"
#include "../Utils/Math/Matrix.h"

#include "Interpolators/InterpolatorFactory.h"

class Model;
class Mesh;

class Animation
{
public:
	int id;
	
	sm::Matrix worldTMInv;
	
	sm::Vec3 localPos;
	sm::Quat localRot;
	sm::Vec3 localScale;
	
	IInterpolator<sm::Vec3> *pos;
	IInterpolator<sm::Quat> *rot;
	IInterpolator<sm::Vec3> *scale;
	
	std::vector<Animation*> subAnims;
	
	Mesh *mesh;
	
public:
	Animation(void);
	~Animation(void);
	
	void AssignModel(Model *model);
	void Update(float time, const sm::Matrix &transform);
	
	Animation *GetAnimationById(int id);
	float GetAnimLength(int id);
};
