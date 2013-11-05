#pragma once

#import "../Utils/Math/Vec3.h"

class BoundingBox
{
public:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	sm::Vec3 center;

	/*static BoundingBox FromSgmGeometry(const sgm::SgmGeometry *geo)
	{
		BoundingBox bbox;

		bbox.maxX = geo ->meshes[0] ->vertices[0].position.x;
		bbox.minX = geo ->meshes[0] ->vertices[0].position.x;
		bbox.maxY = geo ->meshes[0] ->vertices[0].position.y;
		bbox.minY = geo ->meshes[0] ->vertices[0].position.y;
		bbox.maxZ = geo ->meshes[0] ->vertices[0].position.z;
		bbox.minZ = geo ->meshes[0] ->vertices[0].position.z;

		for (int i = 0; i < geo ->meshesCount; i++)
		{
			sgm::SgmMesh *mesh = geo ->meshes[i];

			for (int j = 0; j < mesh ->verticesCount; j++)
			{
				if (bbox.minX > mesh ->vertices[j].position.x) bbox.minX = mesh ->vertices[j].position.x;
				if (bbox.maxX < mesh ->vertices[j].position.x) bbox.maxX = mesh ->vertices[j].position.x;
				if (bbox.minY > mesh ->vertices[j].position.y) bbox.minY = mesh ->vertices[j].position.y;
				if (bbox.maxY < mesh ->vertices[j].position.y) bbox.maxY = mesh ->vertices[j].position.y;
				if (bbox.minZ > mesh ->vertices[j].position.z) bbox.minZ = mesh ->vertices[j].position.z;
				if (bbox.maxZ < mesh ->vertices[j].position.z) bbox.maxZ = mesh ->vertices[j].position.z;
			}
		}

		bbox.center.Set(
			(bbox.maxX + bbox.minX) / 2.0f,
			(bbox.maxY + bbox.minY) / 2.0f,
			(bbox.maxZ + bbox.minZ) / 2.0f);

		return bbox;
	}

	sm::Vec3 GetVerticle(int index)
	{
		sm::Vec3 verticle;

		switch (index)
		{
		case 0: verticle.Set(minX, maxY, minZ); break;
		case 1: verticle.Set(maxX, maxY, minZ); break;
		case 2: verticle.Set(maxX, maxY, maxZ); break;
		case 3: verticle.Set(minX, maxY, maxZ); break;
		case 4: verticle.Set(minX, minY, minZ); break;
		case 5: verticle.Set(maxX, minY, minZ); break;
		case 6: verticle.Set(maxX, minY, maxZ); break;
		case 7: verticle.Set(minX, minY, maxZ); break;
		}

		return verticle;
	}*/
};
