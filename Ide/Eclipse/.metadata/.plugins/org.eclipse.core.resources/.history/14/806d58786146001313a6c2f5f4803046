#pragma once

#include <OpenGLES/ES1/gl.h>
#include <OpenGLES/ES1/glext.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#import "../Utils/Math/Matrix.h"
#import "../Utils/Math/Vec3.h"
#import "../Utils/Math/Vec2.h"

class SteerCamera
{
private:
	sm::Vec3 position;

	float roll;
	float yaw;
	float pitch;

	sm::Vec2 *lastMousePos;
	sm::Vec2 screenCentre;
	sm::Vec2 lastPos;

public:
	SteerCamera();
	~SteerCamera();

	void Process(float ms);
	void SetViewMatrix();
	sm::Matrix GetViewMatrix();
	sm::Vec3 GetPosition();
};
