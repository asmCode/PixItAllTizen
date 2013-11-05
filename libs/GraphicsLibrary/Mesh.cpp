#include "Mesh.h"

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import "../Utils/Math/Matrix.h"
#include "Vertex.h"
#include "Effect.h"
#include "Material.h"
#include "MeshPart.h"

Mesh::Mesh()
{
	nodeId = 0;
	transform = sm::Matrix::IdentityMatrix();
}

Mesh::~Mesh()
{
	for (int i = 0; i < (int)meshParts.size(); i++)
		delete meshParts[i];

	meshParts.clear();
}

std::vector<MeshPart*> Mesh::GetMeshParts()
{
	return meshParts;
}

void Mesh::AddMeshPart( MeshPart *meshPart )
{
	meshParts.push_back(meshPart);
}

sm::Matrix& Mesh::Transform()
{
	return transform;
}