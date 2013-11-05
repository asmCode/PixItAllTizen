#include "MeshPart.h"
//#include <OpenGLES/ES1/gl.h>
//#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>


MeshPart::MeshPart(int verticesCount, Vertex *vertices)
{
	this ->verticesCount = verticesCount;
	this ->vertices = vertices;

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex),
		vertices, GL_STATIC_DRAW);


	int positionOffset = 0;
	int normalOffset = positionOffset + sizeof(sm::Vec3);
	int tangentOffset = normalOffset + sizeof(sm::Vec3);
	int coordsOffset = tangentOffset + sizeof(sm::Vec3);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_NORMAL_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);	
//	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (void*)positionOffset);
//	glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)normalOffset);
//	glClientActiveTexture(GL_TEXTURE0);
//	glTexCoordPointer(4, GL_FLOAT, sizeof(Vertex), (void*)tangentOffset);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//	glClientActiveTexture(GL_TEXTURE1);
//	glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)coordsOffset);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	material = NULL;
	//effect = NULL;
}

MeshPart::~MeshPart()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vboId);

	delete [] vertices;
}

void MeshPart::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	int positionOffset = 0;
	int normalOffset = positionOffset + sizeof(sm::Vec3);
	int tangentOffset = normalOffset + sizeof(sm::Vec3);
	int coordsOffset = tangentOffset + sizeof(sm::Vec3);
	
	glVertexAttribPointer(0, 4, GL_FLOAT, false, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 4, GL_FLOAT, false, sizeof(Vertex), (void*)normalOffset);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 4, GL_FLOAT, false, sizeof(Vertex), (void*)tangentOffset);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(Vertex), (void*)coordsOffset);
	glEnableVertexAttribArray(3);
	
	//	glVertexPointer(4, GL_FLOAT, sizeof(Vertex), (void*)positionOffset);
	//glNormalPointer(GL_FLOAT, sizeof(Vertex), (void*)normalOffset);
	//glClientActiveTexture(GL_TEXTURE0);
	//glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)tangentOffset);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glClientActiveTexture(GL_TEXTURE1);
	//glTexCoordPointer(3, GL_FLOAT, sizeof(Vertex), (void*)coordsOffset);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount);
	
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glClientActiveTexture(GL_TEXTURE0);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glClientActiveTexture(GL_TEXTURE1);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
void MeshPart::SetEffect(Effect *effect)
{
	this ->effect = effect;
}

Effect *MeshPart::GetEffect()
{
	return effect;
}
*/

void MeshPart::SetMaterial(Material *material)
{
	this ->material = material;
}

Material *MeshPart::GetMaterial()
{
	return material;
}

int MeshPart::GetVerticesCount()
{
	return verticesCount;
}

const Vertex* MeshPart::GetVertices()
{
	return vertices;
}
