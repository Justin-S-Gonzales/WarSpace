#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

class Shader
{
public:
	Shader();

	void CreateFromFiles(const char* vertexLocation, const char* fragmentLocation);

	void UseShader();
	void Validate();

	GLuint GetShaderID() { return m_ShaderID; }
	GLuint GetProjectionLocation();
	GLuint GetModelLocation();

	void ClearShader();

	~Shader();

private:
	std::string ReadFile(const char* fileLocation);
	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
	void CompileProgram();

	GLuint m_ShaderID;
	
	// Uniforms
	GLuint m_UProjection, m_UModel;
};

