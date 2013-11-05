#include "Animation.h"
#include "Model.h"
#include "Mesh.h"

Animation::Animation(void)
{
	nodeId = 0;
	mesh = NULL;
	worldTMInv = sm::Matrix::IdentityMatrix();

	pos = NULL;
	rot = NULL;
	scale = NULL;

	localPos.Set(0, 0, 0);
	localRot.RotateToQuat(0, 0, 0, 0);
	localScale.Set(1, 1, 1);
}

Animation::~Animation(void)
{
	if (pos != NULL)
		delete pos;

	if (rot != NULL)
		delete rot;

	if (scale != NULL)
		delete scale;

	for (unsigned i = 0; i < subAnims.size(); i++)
		delete subAnims[i];
}

void Animation::AssignModel(Model *model)
{
	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->AssignModel(model);

	if (nodeId == 0)
		return;

	std::vector<Mesh*> &meshes = model ->GetMeshes();
	for (unsigned i = 0; i < meshes.size(); i++)
	{
		if (meshes[i] ->nodeId == nodeId)
		{
			mesh = meshes[i];
			return;
		}
	}
}

void Animation::Update(float time, const sm::Matrix &transform)
{
	sm::Matrix tr = transform;

	sm::Vec3 posVal;
	sm::Quat rotVal;
	sm::Vec3 scaleVal;

	float angle;
	sm::Vec3 axis;

	if (pos != NULL)
		pos ->GetValue(time, posVal);
	else
		posVal = localPos;

	if (rot != NULL)
		rot ->GetValue(time, rotVal);
	else
		rotVal = localRot;

	if (scale != NULL)
		scale ->GetValue(time, scaleVal);
	else
		scaleVal = localScale;

	rotVal.Normalize();
	rotVal.QuatToRotate(angle, axis);

	tr *= sm::Matrix::TranslateMatrix(posVal);
	tr *= sm::Matrix::ScaleMatrix(scaleVal);
	tr *= sm::Matrix::RotateAxisMatrix(angle, axis);

	if (mesh != NULL)
	{
		mesh ->Transform() = tr;
		mesh ->Transform() *= worldTMInv;
	}

	for (unsigned i = 0; i < subAnims.size(); i++)
		subAnims[i] ->Update(time, tr);
}
