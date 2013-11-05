#pragma once

#include "IAssetLoader.h"
#include "../Graphics/Model.h"
#include <fstream>

class BinaryReader;

class ModelLoader : public IAssetLoader
{
private:
	Mesh *LoadMesh(BinaryReader *br);
	MeshPart *LoadMeshPart(BinaryReader *br);

public:
	ModelLoader();
	
	int EstimageProgress(const char *path);
	void *Load(const char *path);
};
