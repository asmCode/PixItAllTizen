#include "Model.h"

Model::Model()
{
}

Model::~Model()
{
	for (int i = 0; i < (int)meshes.size(); i++)
		delete meshes[i];

	meshes.clear();
}

std::vector<Mesh*> &Model::GetMeshes()
{
	return meshes;
}

void Model::GetMeshParts(std::vector<MeshPart*> &meshParts)
{
	for (unsigned i = 0; i < meshes.size(); i++)
		for (unsigned j = 0; j < meshes[i]->GetMeshParts().size(); j++)
			meshParts.push_back(meshes[i]->GetMeshParts()[j]);
}
