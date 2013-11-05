#pragma once

#import <vector>
#import "../Utils/Math/Vec3.h"
#import "../Utils/Math/Quat.h"
#import "../Utils/Math/Matrix.h"
#import "../Utils/IDisposable.h"

#include "Interpolators/InterpolatorFactory.h"

class Model;
class Mesh;

class Animation : public IDisposable
{
public:
	int nodeId;

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
};
