#include "Effect.h"

/*Effect::Effect(CGeffect cgEffect)
{
	this ->cgEffect = cgEffect;

	currentTech = NULL;

	CGtechnique tech = cgGetFirstTechnique(cgEffect);
	while (tech != NULL)
	{
		const char *techName = cgGetTechniqueName(tech);
		techniques[techName] = tech;

		tech = cgGetNextTechnique(tech);
	}
}

Effect::~Effect(void)
{
	cgDestroyEffect(cgEffect);
}

void Effect::SetCurrentTechnique(const char *name)
{
	currentTech = techniques[name];

	currentTechPasses.clear();

	CGpass pass = cgGetFirstPass(currentTech);
	while (pass != NULL)
	{
		currentTechPasses.push_back(pass);

		pass = cgGetNextPass(pass);
	}
}

void Effect::SetWorld(sm::Matrix world)
{
	//vShader ->SetParameter("World", world);
}

void Effect::SetView(sm::Matrix view)
{
	//vShader ->SetParameter("View", view);
}

void Effect::SetProj(sm::Matrix proj)
{
	//vShader ->SetParameter("Proj", proj);
}

int Effect::GetPassesCount()
{
	return (int)currentTechPasses.size();
}

void Effect::BeginPass(int passIndex)
{
	cgSetPassState(currentTechPasses[passIndex]);
}

void Effect::EndPass(int passIndex)
{
	cgResetPassState(currentTechPasses[passIndex]);
}

void Effect::SetParameter(const char *name, float val)
{
	CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgGLSetParameter1f(parameter, val);
}

void Effect::SetParameter(const char *name, float val1, float val2)
{
CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgGLSetParameter2f(parameter, val1, val2);
}

void Effect::SetParameter(const char *name, float val1, float val2, float val3)
{
	CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgGLSetParameter3f(parameter, val1, val2, val3);
}

void Effect::SetParameter(const char *name, float val1, float val2, float val3, float val4)
{
	CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgGLSetParameter4f(parameter, val1, val2, val3, val4);
}

void Effect::SetParameter(const char *name, const sm::Vec2 &val)
{
	SetParameter(name, val.x, val.y);
}

void Effect::SetParameter(const char *name, const sm::Vec3 &val)
{
	SetParameter(name, val.x, val.y, val.z);
}

void Effect::SetParameter(const char *name, const sm::Matrix &val)
{
	CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgSetMatrixParameterfc(parameter, val);
}

void Effect::SetTextureParameter(const char *name, int texId)
{
	CGparameter parameter = cgGetNamedEffectParameter(cgEffect, name);
	cgGLSetTextureParameter(parameter, texId);
	cgSetSamplerState(parameter);
}*/