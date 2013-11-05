#pragma once
/*
#include <windows.h>
#include <cg\cg.h>
#include <cg\cggl.h>
#include <fstream>

#include <Math\Vec2.h>
#include <Math\Vec3.h>
#include <Math\Matrix.h>

class Shader
{
private:
	CGprogram programId;
	CGprofile profile;
	CGcontext context;

public:
	Shader(CGcontext context, CGprofile profile, const char *filename);
	~Shader(void);

	virtual void Begin();
	void End();

	CGprofile GetProfile();
	CGprogram GetProgram();

	void SetParameter(const char *name, float val);
	void SetParameter(const char *name, float val1, float val2);
	void SetParameter(const char *name, float val1, float val2, float val3);
	void SetParameter(const char *name, float val1, float val2, float val3, float val4);
	void SetParameter(const char *name, sm::Vec2 val);
	void SetParameter(const char *name, sm::Vec3 val);
	void SetParameter(const char *name, sm::Matrix val);

	void SetCurrentModelviewMatrix(const char *name);
	void SetCurrentModelviewProjectionMatrix(const char *name);
	void SetTextureParameter(const char *name, int texId);
};
*/