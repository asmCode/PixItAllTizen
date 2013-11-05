#pragma once

#include "MeshPart.h"
#include "../Utils/Math/Matrix.h"
#include <vector>
#include <string>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

class Mesh
{
private:
	std::vector<MeshPart*> meshParts;
	sm::Matrix transform;

public:
	int id;
	std::string name;

	Mesh();
	~Mesh();

	void AddMeshPart(MeshPart *meshPart);
	std::vector<MeshPart*>& GetMeshParts();

	bool IsCam();
	int GetId();
	sm::Matrix& Transform();

	sm::Vec3 colorMask;
};
