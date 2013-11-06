#include "ModelLoader.h"
#include "../Utils/Utils/BinaryReader.h"
#include "../Graphics/Vertex.h"
#include "../Graphics/MeshPart.h"
#include "../Graphics/Mesh.h"

ModelLoader::ModelLoader()
{
}

int ModelLoader::EstimageProgress(const char *path)
{
	std::ifstream file(path, std::ios_base::binary);
	int meshesCount;
	file.read((char*)&meshesCount, sizeof(int));
	file.close();

	return meshesCount;
}

void *ModelLoader::Load(const char *path)
{
	std::ifstream file(path, std::ios_base::binary);
	file.seekg(0, std::ios::end);
	unsigned fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	char *buff = new char[fileSize];
	file.read(buff, fileSize);
	file.close();

	Model *model = new Model();
	std::vector<Mesh*> &meshes = model ->GetMeshes();

	BinaryReader *br = new BinaryReader(buff);

	int meshesCount = br ->Read<int>();

	for (int i = 0; i < meshesCount; i++)
	{
		meshes.push_back(LoadMesh(br));
	}

	//delete br;
	delete [] buff;

	return model;
}

Mesh *ModelLoader::LoadMesh(BinaryReader *br)
{
	Mesh *mesh = new Mesh();
	mesh ->id = br ->Read<int>();
	mesh->name = br->Read<std::string>();

	int meshPartsCount = br ->Read<int>();

	for (int i = 0; i < meshPartsCount; i++)
	{
		MeshPart *meshPart = LoadMeshPart(br);
		meshPart->m_mesh = mesh;
		mesh ->AddMeshPart(meshPart);
	}

	return mesh;
}

MeshPart *ModelLoader::LoadMeshPart(BinaryReader *br)
{
	std::string materialName = br ->Read<std::string>();

	int verticesCount = br ->Read<int>();
	Vertex *vertices = new Vertex[verticesCount];

	for (int i = 0; i < verticesCount; i++)
	{
		vertices[i].position.x = br->Read<float>();
		vertices[i].position.y = br->Read<float>();
		vertices[i].position.z = br->Read<float>();

		vertices[i].normal.x = br ->Read<float>();
		vertices[i].normal.y = br ->Read<float>();
		vertices[i].normal.z = br ->Read<float>();

		vertices[i].tangent.x = br ->Read<float>();
		vertices[i].tangent.y = br ->Read<float>();
		vertices[i].tangent.z = br ->Read<float>();

		vertices[i].texCoord.x = br ->Read<float>();
		vertices[i].texCoord.y = br ->Read<float>();
	}

	MeshPart *meshPart = new MeshPart(verticesCount, vertices, NULL);
	meshPart ->materialName = materialName;

	return meshPart;
}
