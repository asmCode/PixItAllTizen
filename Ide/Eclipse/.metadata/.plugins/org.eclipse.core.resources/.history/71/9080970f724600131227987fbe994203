#include "ParticleManager.h"
#include <Utils/Utils/Randomizer.h>
#include "ClassContainer.h"
#include <Graphics/Color.h>

const Color ParticleManager::PrettyColors[] = 
{
	Color(53, 140, 193),
	Color(151, 184, 219),
	Color(0, 75, 136),
	Color(90, 184, 182),
	Color(150, 204, 233),
	Color(207, 230, 245),
	Color(0, 150, 79),
	Color(0, 111, 59),
	Color(235, 238, 48),
	Color(231, 0, 188),
	Color(96, 0, 73),
	Color(254, 181, 0),
	Color(251, 87, 0),
	Color(252, 10, 35),
	Color(254, 181, 1),
	Color(254, 134, 1),
	Color(212, 20, 90),
	Color(192, 204, 10),
	Color(233, 247, 229),
	Color(201, 233, 191),
	Color(188, 222, 89) // 21 colors
};

ParticleManager::ParticleManager()
{
	activeParticles = 0;
	particles = NULL;
	particlesCount = 0;
	isActive = false;
}

ParticleManager::~ParticleManager()
{
	if (particles != NULL)
	{
		for (unsigned i = 0; i < particlesCount; i++)
			delete particles[i];
		
		delete [] particles;
	}
}

void ParticleManager::Update(float time, float seconds)
{
	if (progressTime >= duration)
		return;
	
	progressTime += seconds;
	
	activeParticles = particlesCount;
	
	for (unsigned i = 0; i < particlesCount; i++)
	{
		if (particles[i]->IsAlive())
			particles[i]->Update(time, seconds);
		else
			//StartParticle(particles[i]);
			activeParticles--;
	}
}

void ParticleManager::Update(float time, float seconds, const sm::Matrix &trans)
{
	Update(time, seconds);
	
	if (progressTime >= duration)
		return;
	
	for (unsigned i = 0; i < particlesCount; i++)
		particles[i]->Transform() = trans * particles[i]->Transform();
		//particles[i]->Transform() = particles[i]->Transform() * trans;
}

void ParticleManager::Draw(float time, float seconds)
{
	if (progressTime >= duration)
		return;

	for (unsigned i = 0; i < particlesCount; i++)
		particles[i]->Draw(time, seconds);
}

void ParticleManager::Initialize(unsigned particlesCount, sm::Vec2 sourcePos)
{
	this->particlesCount = particlesCount;
	this->sourcePos = sourcePos;
	
	particles = new Particle * [particlesCount];
	
	for (unsigned i = 0; i < particlesCount; i++)
	{
		particles[i] = new Particle(ClassContainer::GetInstance()->GetClass<TexPart>("guimap_drop_body"),
									ClassContainer::GetInstance()->GetClass<TexPart>("guimap_drop_shiny"));
	}
}

Color ParticleManager::GetRandomColor()
{
	return PrettyColors[rand() % 21];
}

void ParticleManager::StartParticle(Particle *particle, float duration, float speed, float scale)
{
	static Randomizer random;
	
	particle->Start(GetRandomColor(),
					random.GetFloat(scale * 0.2f, scale), // scale
					random.GetFloat(duration * 0.5f, duration), // life time
					sourcePos,
					sm::Vec2(random.GetFloat(-1.0f, 1.0), random.GetFloat(-1.0f, 1.0)).GetNormalized(),
					random.GetFloat(speed / 4.0f, speed), // speed
					random.GetFloat(0, 3.1415f),
					random.GetInt(0, 100) % 2 ? -1 : 1,
					random.GetFloat(3.0f, 6.0f)); // rotate speed
}

void ParticleManager::Blow(float duration, float speed, float scale)
{
	progressTime = 0;
	this->duration = duration;
	
	activeParticles = particlesCount;
	
	for (unsigned i = 0; i < particlesCount; i++)
		StartParticle(particles[i], duration, speed, scale);
}

bool ParticleManager::IsActive() const
{
	return activeParticles > 0;
}

void ParticleManager::SetPosition(sm::Vec2 position)
{	
	for (unsigned i = 0; i < particlesCount; i++)
		particles[i]->SetPosition(particles[i]->GetPosition() + (position - sourcePos));
	
	sourcePos = position;
}

sm::Vec2 ParticleManager::GetPosition() const
{
	return sm::Vec2(sourcePos.x, sourcePos.y);
}

void ParticleManager::Stop()
{	
	for (unsigned i = 0; i < particlesCount; i++)
		particles[i]->Stop();
	
	activeParticles = 0;
}
