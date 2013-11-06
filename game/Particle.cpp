#include "Particle.h"

Particle::Particle(TexPart tpDropBody,
				   TexPart tpDropShiny)
{
	lifeTime = 0;
	lifeTimeLimit = 0;
	trans = sm::Matrix::IdentityMatrix();
	
	this->tpDropBody = tpDropBody;
	this->tpDropShiny = tpDropShiny;
}

void Particle::Start(Color color,
					 float scale,
					 float lifeTimeLimit,
					 sm::Vec2 sourcePosition,
					 sm::Vec2 target,
					 float speed,
					 float rotAngle,
					 float rotAngleDir,
					 float rotSpeed)
{
	lifeTime = 0;
	this->color = color;
	this->scale = scale;
	this->lifeTimeLimit = lifeTimeLimit;
	this->target = sm::Vec3(target.x, target.y, 0);
	this->position = sm::Vec3(sourcePosition.x, sourcePosition.y, 0);
	this->speed = speed;
	this->rotAngle = rotAngle;
	this->rotAngleDir = rotAngleDir;
	this->rotSpeed = rotSpeed;
}

void Particle::Update(float time, float seconds)
{
	lifeTime += seconds;
	position += target * speed * seconds;
	rotAngle += rotAngleDir * rotSpeed * seconds;
	
	float lifeProgress = lifeTime / lifeTimeLimit;
	if (lifeProgress > 0.5f)
		opacity = (1.0f - (fminf(1.0f, (lifeProgress - 0.5f) / 0.5f))) * 255.0f;
	else
		opacity = 255.0f;
	
	trans =
		sm::Matrix::TranslateMatrix(position) *
		sm::Matrix::RotateAxisMatrix(rotAngle, 0, 0, 1) *
		sm::Matrix::ScaleMatrix(tpDropBody.ImageRect.Width * scale, tpDropBody.ImageRect.Height * scale, 1);	
}

void Particle::Draw(float time, float seconds)
{
	sb.Draw(tpDropBody, Color(color.R, color.G, color.B, (unsigned char)opacity), trans);
	sb.Draw(tpDropShiny, Color(255, 255, 255, (unsigned char)((float)opacity * 0.5f)), trans * sm::Matrix::ScaleMatrix(0.9f, 0.9f, 1.0f));
}

void Particle::SetLifetime(float lifeTime)
{
	this->lifeTime = lifeTime;
}

sm::Matrix& Particle::Transform()
{
	return trans;
}

bool Particle::IsAlive() const
{
	return lifeTime < lifeTimeLimit;
}

void Particle::SetPosition(sm::Vec2 position)
{
	this->position.x = position.x;
	this->position.y = position.y;
}

sm::Vec2 Particle::GetPosition() const
{
	return sm::Vec2(position.x, position.y);
}

void Particle::Stop()
{
	lifeTime = lifeTimeLimit;
}