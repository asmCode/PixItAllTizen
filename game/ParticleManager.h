#pragma once

#include <Utils/Math/Vec2.h>
#include <GraphicsLibrary/IDrawable.h>
#include <GraphicsLibrary/IUpdateable.h>
#include "Particle.h"
#include <stddef.h>

class ParticleManager :
	public IDrawable,
	public IUpdateable
{
private:
	static const Color PrettyColors[];
	
	Particle **particles;
	unsigned particlesCount;
	sm::Vec2 sourcePos;
	
	void StartParticle(Particle *particle, float duration, float speed, float scale);
	Color GetRandomColor();
	
	unsigned activeParticles;
	bool isActive;
	float duration;
	float progressTime;
	
public:
	ParticleManager();
	~ParticleManager();
	
	void Initialize(unsigned particlesCount, sm::Vec2 sourcePos);
	
	void Update(float time, float seconds);
	void Update(float time, float seconds, const sm::Matrix &trans);
	void Draw(float time, float seconds);
	
	void Blow(float duration, float speed, float scale);
	void Stop();
	bool IsActive() const;
	
	void SetPosition(sm::Vec2 position);
	
	sm::Vec2 GetPosition() const;
};
