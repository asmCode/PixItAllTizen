#include "Shader.h"
#include <fstream>
#include <assert.h>

Shader::Shader() :
	m_programId(0),
	m_vertShaderId(0),
	m_fragShaderId(0)
{
}

Shader::~Shader()
{
}

Shader* Shader::LoadFromFile(const char *vertexShaderFile,
							 const char *fragmentShaderFile)
{	
	Shader *shader = new Shader();
	
	shader->m_vertShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderFile);
	if (shader->m_vertShaderId == 0)
	{
		assert(false);
		delete shader;
		return NULL;
	}
	
	shader->m_fragShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderFile);
	if (shader->m_fragShaderId == 0)
	{
		assert(false);
		delete shader;
		return NULL;
	}
	
	shader->m_programId = glCreateProgram();
	
	glAttachShader(shader->m_programId, shader->m_vertShaderId);
	glAttachShader(shader->m_programId, shader->m_fragShaderId);
	
	glBindAttribLocation(shader->m_programId, 0, "position");
	glBindAttribLocation(shader->m_programId, 1, "normal");
	glBindAttribLocation(shader->m_programId, 2, "coords");
	
	GLint status;
    glLinkProgram(shader->m_programId);
    
#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(shader->m_programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(shader->m_programId, logLength, &logLength, log);
        printf("Program link log:\n%s", log);
        free(log);
    }
#endif
    
    glGetProgramiv(shader->m_programId, GL_LINK_STATUS, &status);
    if (status == 0)
	{
        return NULL;
    }
	
	ValidateProgram(shader->m_programId);
	
	return shader;
}

void Shader::UseProgram()
{
	glUseProgram(m_programId);
}

void Shader::BindVertexChannel(unsigned channel, const char *name)
{
}

void Shader::SetParameter(const char *name, float val)
{
}

void Shader::SetParameter(const char *name, float val1, float val2, float val3)
{	
}

void Shader::SetParameter(const char *name, float val1, float val2, float val3, float val4)
{
}

void Shader::SetParameter(const char *name, const sm::Vec3 &val)
{	
}

void Shader::SetTextureParameter(const char *name, unsigned channel, unsigned texId)
{
	glActiveTexture(GL_TEXTURE0 + channel);
	glBindTexture(GL_TEXTURE_2D, texId);

	int uniformParam = glGetUniformLocation(m_programId, name);
	assert(uniformParam != -1);
	
	glUniform1i(uniformParam, channel);

}

void Shader::SetMatrixParameter(const char *name, const sm::Matrix &matrix)
{
	int uniformParam = glGetUniformLocation(m_programId, name);
	assert(uniformParam != -1);
	
	glUniformMatrix4fv(uniformParam, 1, false, matrix);
}

GLuint Shader::CompileShader(GLenum shaderType, const char* file)
{
	std::ifstream fileStream(file);
	if (!fileStream.is_open() || fileStream.fail())
	{
		assert(false);
		return NULL;
	}
	
	fileStream.seekg(0, std::ios::end);
	unsigned fileSize = fileStream.tellg();
	fileStream.seekg(0, std::ios::beg);
	
	char *fileContent = new char[fileSize + 1];
	fileStream.read(fileContent, fileSize);
	fileStream.close();
	fileContent[fileSize] = 0;
	
	GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, (const GLchar**)&fileContent, NULL);
    glCompileShader(shaderId);
    
#ifdef DEBUG
    GLint logLength;
    glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0) {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(shaderId, logLength, &logLength, log);
		printf("%s", log);
        free(log);
    }
#endif
    
	GLint status;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
    if (status == 0)
	{
		glDeleteShader(shaderId);
        return 0;
    }
    
    return shaderId;
}

bool Shader::ValidateProgram(GLuint programId)
{
	GLint logLength;
	GLint status;
    
    glValidateProgram(programId);
    glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
	{
		GLchar *_log = new GLchar[logLength];
        glGetProgramInfoLog(programId, logLength, &logLength, _log);
		printf("validate log: %s\n", _log);
        delete _log;
		assert(false);
    }
    
    glGetProgramiv(programId, GL_VALIDATE_STATUS, &status);
    return status == 0;
}
