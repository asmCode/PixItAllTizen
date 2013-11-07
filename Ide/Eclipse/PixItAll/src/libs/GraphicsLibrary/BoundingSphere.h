#pragma once

#import "../Utils/Math/Vec3.h"
#import "../Utils/Math/Matrix.h"
#import "BoundingBox.h"

class BoundingSphere
{
public:
	sm::Vec3 center;
	float radiusLength;

	//static BoundingSphere FromSgmGeometry(const sgm::SgmGeometry *geo)
	//{
	//	BoundingSphere bs;

	//	BoundingBox bb = BoundingBox::FromSgmGeometry(geo);

	//	bs.center = bb.center;

	//	for (int i = 0; i < geo ->meshesCount; i++)
	//	{
	//		sgm::SgmMesh *mesh = geo ->meshes[i];

	//		for (int j = 0; j < mesh ->verticesCount; j++)
	//		{
	//			float radiusLength = (bs.center - mesh ->vertices[j].position).GetLength();
	//			if (bs.radiusLength < radiusLength)
	//				bs.radiusLength = radiusLength;
	//		}
	//	}

	//	return bs;
	//}

	//static bool IsCollision(BoundingSphere bs1, BoundingSphere bs2)
	//{
	//	return (bs1.center - bs2.center).GetLength() < (bs1.radiusLength + bs2.radiusLength);
	//}

	//BoundingSphere Transform(sm::Matrix transform)
	//{

	//	//TODO: tu pewnie sa zle obliczenia. Do radiusa trzeba dodac center
	//	BoundingSphere bs;

	//	sm::Vec3 radius(radiusLength, 0, 0);
	//	radius = transform * radius;
	//	bs.center = transform * center;
	//	bs.radiusLength = (bs.center - radius).GetLength();

	//	return bs;
	//}
};
