#pragma once

#include <vector>

//#include "GlExtFactory.h"
#include "Mesh.h"

class Model
{
private:
	std::vector<Mesh*> meshes;

public:
	Model();
	~Model();

	std::vector<Mesh*> &GetMeshes();
	void GetMeshParts(std::vector<MeshPart*> &meshParts);
};
