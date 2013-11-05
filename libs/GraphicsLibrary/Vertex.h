#pragma once

#import "../Utils/Math/Vec3.h"
#import "../Utils/Math/Vec2.h"

class Vertex
{
public:
	sm::Vec3 position;
	sm::Vec3 normal;
	sm::Vec3 tangent;
	sm::Vec2 texCoord;
};
 