#pragma once

#include <vector>
#import "../Utils/Math/Matrix.h"

#include "MeshPart.h"

class Mesh
{
private:
	std::vector<MeshPart*> meshParts;
	sm::Matrix transform;

public:
	int nodeId;

	Mesh();
	~Mesh();

	void AddMeshPart(MeshPart *meshPart);
	std::vector<MeshPart*> GetMeshParts();

	sm::Matrix& Transform();
};
