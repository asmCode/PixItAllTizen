#pragma once

#include "Vertex.h"
#include "Effect.h"
#include "Material.h"

#include "Utils.h"
#include <string>

class MeshPart
{
public:
	unsigned vboId;
	Material *material;
	//Effect *effect;

	int verticesCount;
	Vertex *vertices;

public:
	std::string materialName;

	MeshPart(int verticesCount, Vertex *vertices);
	~MeshPart();

//	void SetEffect(Effect *effect);
//	Effect *GetEffect();

	void SetMaterial(Material *material);
	Material *GetMaterial();

	int GetVerticesCount();
	const Vertex* GetVertices();

	void Draw();
};
