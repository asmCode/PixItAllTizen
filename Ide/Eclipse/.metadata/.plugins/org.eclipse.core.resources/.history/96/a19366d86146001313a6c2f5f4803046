/* #include "Shader.h"

Shader::Shader(CGcontext context, CGprofile profile, const char *filename)
{
	this ->context = context;
	this ->profile = profile;
	
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	int fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	char *code = new char[fileSize + 1];
	code[fileSize] = 0;
	file.read(code, fileSize);
	file.close();

	programId = cgCreateProgram(context, CG_SOURCE, code, profile, "main", NULL);
	const char *zlo = cgGetErrorString(cgGetError());
	cgGLLoadProgram(programId);

	delete [] code;
}

Shader::~Shader(void)
{
	if (programId != NULL)
		cgDestroyProgram(programId);
}

void Shader::Begin()
{
	cgGLEnableProfile(profile);
	cgGLBindProgram(programId);
}

void Shader::End()
{
	//cgGLEnableProfile(0);
	//cgGLBindProgram(0);
}

CGprofile Shader::GetProfile()
{
	return profile;
}

CGprogram Shader::GetProgram()
{
	return programId;
}

void Shader::SetParameter(const char *name, float val)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetParameter1f(parameter, val);
}

void Shader::SetParameter(const char *name, float val1, float val2)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetParameter2f(parameter, val1, val2);
}

void Shader::SetParameter(const char *name, float val1, float val2, float val3)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetParameter3f(parameter, val1, val2, val3);
}

void Shader::SetParameter(const char *name, float val1, float val2, float val3, float val4)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetParameter4f(parameter, val1, val2, val3, val4);
}

void Shader::SetParameter(const char *name, sm::Vec2 val)
{
	SetParameter(name, val.x, val.y);
}

void Shader::SetParameter(const char *name, sm::Vec3 val)
{
	SetParameter(name, val.x, val.y, val.z);
}

void Shader::SetParameter(const char *name, sm::Matrix val)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetMatrixParameterfc(parameter, val);
}

void Shader::SetCurrentModelviewMatrix(const char *name)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetStateMatrixParameter(parameter, CG_GL_MODELVIEW_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void Shader::SetCurrentModelviewProjectionMatrix(const char *name)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetStateMatrixParameter(parameter, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);
}

void Shader::SetTextureParameter(const char *name, int texId)
{
	cgGLBindProgram(programId);

	CGparameter parameter = cgGetNamedParameter(programId, name);
	cgGLSetTextureParameter(parameter, texId);
	cgGLEnableTextureParameter(parameter);
}
*/