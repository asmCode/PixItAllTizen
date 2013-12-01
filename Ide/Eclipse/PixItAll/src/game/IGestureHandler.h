#pragma once

#include <Utils/Math/Vec2.h>
#include <GraphicsLibrary/Point.h>

class IGestureHandler
{
public:
	enum GestureStatus
	{
		GestureStatus_Began,
		GestureStatus_Changed,
		GestureStatus_Ended
	};
	
	virtual ~IGestureHandler() {}

	virtual void HandlePinchGesture(IGestureHandler::GestureStatus status, float scale, float velocity) {}
	virtual void HandlePanGesture(IGestureHandler::GestureStatus status,
								  const sm::Vec2 &pos,
								  const sm::Vec2 &trans,
								  const sm::Vec2 &velocity) {}
	virtual void HandleTapGesture(const sm::Point<int> &point) {}
	virtual void HandlePress(const sm::Point<int> &point) {}
	virtual void HandleRelease(const sm::Point<int> &point) {}
};
