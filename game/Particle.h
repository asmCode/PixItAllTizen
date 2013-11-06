#pragma once

#include <Utils/Math/Matrix.h>
#include <Utils/Math/Vec2.h>
#include <Utils/Math/Vec3.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/IUpdateable.h>
#include <GraphicsLibrary/SpriteBatch.h>
#include <GraphicsLibrary/TexPart.h>

class Particle :
	public IUpdateable,
	public IDrawable
{
private:
	SpriteBatch	sb;
	float		lifeTime;
	float		lifeTimeLimit;
	sm::Matrix	trans;
	
	sm::Vec3	target;
	sm::Vec3	position;
	float		speed;
	float		opacity;
	float		rotAngle;
	float		rotAngleDir;
	float		rotSpeed;
	float		scale;
	
	TexPart		tpDropBody;
	TexPart		tpDropShiny;
	Color		color;
	
public:
	Particle(TexPart tpDropBody,
			 TexPart tpDropShiny);
	
	void Start(Color color,
			   float scale,
			   float lifeTimeLimit,
			   sm::Vec2 sourcePosition,
			   sm::Vec2 target,
			   float speed,
			   float rotAngle,
			   float rotAngleDir,
			   float rotSpeed);
	
	void Update(float time, float seconds);
	void Draw(float time, float seconds);
	
	void SetLifetime(float lifeTime);
	void SetPosition(sm::Vec2 position);
	sm::Vec2 GetPosition() const;
	
	bool IsAlive() const;
	void Stop();
	
	sm::Matrix& Transform();
};
