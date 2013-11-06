#pragma once

//#include "GlExtFactory.h"
#include "Vertex.h"
#include "Effect.h"
#include "Material.h"

#include "Utils.h"
#include <string>

class Mesh;
class BoundingBox;
class BoundingSphere;

class MeshPart
{
public:
	unsigned int vboId;
	unsigned int m_vertexArrayId;
	Material *material;
	//Effect *effect;

	int		m_verticesCount;
	Vertex	*m_vertices;
	Mesh	*m_mesh;

	BoundingBox		*m_bbox;
	BoundingSphere	*m_bsphere;
	
	bool visible;
	bool isAlwaysVisible;

public:
	std::string materialName;

	MeshPart(int verticesCount, Vertex *vertices, Mesh *mesh);
	~MeshPart();

//	void SetEffect(Effect *effect);
//	Effect *GetEffect();

	void SetMaterial(Material *material);
	Material *GetMaterial();

	int GetVerticesCount();
	const Vertex* GetVertices();
	void SetVisibility(bool visible);
	bool IsVisible() const;
	bool& IsAlvaysVisible();

	void Draw();
};
