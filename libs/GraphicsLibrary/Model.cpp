#include "Model.h"

Model::Model()
{
	/*bbox = BoundingBox::FromSgmGeometry(model);
	bsphere = BoundingSphere::FromSgmGeometry(model);*/
}

Model::~Model()
{
	for (int i = 0; i < (int)meshes.size(); i++)
		delete meshes[i];

	meshes.clear();
}

BoundingBox Model::GetBoundingBox()
{
	return bbox;
}

BoundingSphere Model::GetBoundingSphere()
{
	return bsphere;
}

std::vector<Mesh*> &Model::GetMeshes()
{
	return meshes;
}
