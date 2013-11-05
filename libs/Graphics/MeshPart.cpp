#include "MeshPart.h"
#include "Mesh.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

MeshPart::MeshPart(int verticesCount, Vertex *vertices, Mesh *mesh)
{
	m_bbox = new BoundingBox();
	(*m_bbox) = BoundingBox::FromVertices(vertices, verticesCount);
	m_bsphere = new BoundingSphere();
	(*m_bsphere) = BoundingSphere::FromBBox(*m_bbox);

	visible = true;
	isAlwaysVisible = false;

	this->m_mesh = mesh;
	this->m_verticesCount = verticesCount;
	this->m_vertices = vertices;
	
	glGenVertexArraysOES(1, &m_vertexArrayId);
	glBindVertexArrayOES(m_vertexArrayId);
	
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(sm::Vec3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(sm::Vec3) * 3));
	
	glBindVertexArrayOES(0);
	
	material = NULL;
}

bool& MeshPart::IsAlvaysVisible()
{
	return isAlwaysVisible;
}

MeshPart::~MeshPart()
{
	glDeleteBuffers(1, &vboId);
	glDeleteVertexArraysOES(1, &m_vertexArrayId);

	delete [] m_vertices;
	delete m_bbox;
	delete m_bsphere;
}

void MeshPart::Draw()
{
	glBindVertexArrayOES(m_vertexArrayId);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	glDrawArrays(GL_TRIANGLES, 0, m_verticesCount);
}

//void MeshPart::SetEffect(Effect *effect)
//{
//	this ->effect = effect;
//}
//
//Effect *MeshPart::GetEffect()
//{
//	return effect;
//}

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
	return m_verticesCount;
}

const Vertex* MeshPart::GetVertices()
{
	return m_vertices;
}

void MeshPart::SetVisibility(bool visible)
{
	this->visible = visible;
}

bool MeshPart::IsVisible() const
{
	return visible;
}
