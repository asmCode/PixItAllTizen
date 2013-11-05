#include "Mesh.h"

//#include "GlExtFactory.h"
#include "Vertex.h"
#include "Effect.h"
#include "Material.h"

#include "Utils.h"
#include "MeshPart.h"
#include "../Utils/Math/Matrix.h"

Mesh::Mesh()
{
	id = 0;
	colorMask = sm::Vec3(1, 1, 1);
	transform = sm::Matrix::IdentityMatrix();
}

Mesh::~Mesh()
{
	for (int i = 0; i < (int)meshParts.size(); i++)
		delete meshParts[i];

	meshParts.clear();
}

std::vector<MeshPart*>& Mesh::GetMeshParts()
{
	return meshParts;
}

void Mesh::AddMeshPart( MeshPart *meshPart )
{
	meshParts.push_back(meshPart);
}

bool Mesh::IsCam()
{
	return false;
}

int Mesh::GetId()
{
	return id;
}

sm::Matrix& Mesh::Transform()
{
	return transform;
}
