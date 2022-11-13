#include "Shader.h"

Shader::Shader()
{
	m_ShaderID = 0;
	m_UProjection = 0;
	m_UModel = 0;
}

std::string Shader::ReadFile(const char* fileLocation)
{
	std::string content;
	std::ifstream fileStream(fileLocation);

	if (!fileStream.is_open())
	{
		std::cout << "Failed to open file " << fileLocation << "!" << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

void Shader::CreateFromFiles(const char* vertexLocation, const char* fragmentLocation)
{
	std::string vertexString = ReadFile(vertexLocation);
	std::string fragmentString = ReadFile(fragmentLocation);
	const char* vertexCode = vertexString.c_str();
	const char* fragmentCode = fragmentString.c_str();

	CompileShader(vertexCode, fragmentCode);
}

void Shader::AddShader(GLuint program, const char* shaderCode, GLenum shaderType)
{
	// Get index to shader
	GLuint shader = glCreateShader(shaderType);

	const GLchar* code[1]; // equivalent to const GLchar* theCode*;
	code[0] = shaderCode;

	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codeLength); // set the source code for the shader created by glCreateShader to shader code string
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	// Error check that the code was successfully compiled
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error compiling the " << shaderType << " shader: " << eLog << std::endl;
		return;
	}

	// Attach shader to program
	glAttachShader(program, shader);
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
	m_ShaderID = glCreateProgram();

	if (!m_ShaderID)
	{
		std::cout << "Error creating shader program!" << std::endl;
		return;
	}

	AddShader(m_ShaderID, vertexCode, GL_VERTEX_SHADER);
	AddShader(m_ShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	CompileProgram();
}

void Shader::CompileProgram()
{
	// Link program at shader ID
	glLinkProgram(m_ShaderID);

	// Error check that program was linked
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error linking program: " << eLog << std::endl;
		return;
	}

	// Get uniform locations
	m_UProjection = glGetUniformLocation(m_ShaderID, "projection");
	m_UModel = glGetUniformLocation(m_ShaderID, "model");
}

void Shader::Validate()
{
	// Error check that program was linked
	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glValidateProgram(m_ShaderID);
	glGetProgramiv(m_ShaderID, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(m_ShaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error validating program: " << eLog << std::endl;
		return;
	}
}

GLuint Shader::GetProjectionLocation()
{
	return m_UProjection;
}

GLuint Shader::GetModelLocation()
{
	return m_UModel;
}

void Shader::UseShader()
{
	glUseProgram(m_ShaderID);
}

Shader::~Shader()
{
	ClearShader();
}

void Shader::ClearShader()
{
	if (m_ShaderID != 0)
	{
		glDeleteProgram(m_ShaderID);
		m_ShaderID = 0;
	}

	m_UModel = 0;
	m_UProjection = 0;
}