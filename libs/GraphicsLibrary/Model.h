#pragma once

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <vector>
#include "../Utils/IDisposable.h"
#include "BoundingSphere.h"
#include "Mesh.h"

class Model : public IDisposable
{
private:
	std::vector<Mesh*> meshes;

	BoundingBox bbox;
	BoundingSphere bsphere;

public:
	Model();
	~Model();

	BoundingBox GetBoundingBox();
	BoundingSphere GetBoundingSphere();

	std::vector<Mesh*> &GetMeshes();
};
