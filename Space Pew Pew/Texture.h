#pragma once

#include <GL/glew.h>

#include "CommonVals.h"

#include <iostream>

class Texture
{
public: 
	Texture();
	Texture(const char* fileLocation);

	bool LoadTextureA();

	void UseTexture();
	void ClearTexture();

	~Texture();

private:
	GLuint m_TextureID;
	int m_Width, m_Height, m_BitDepth;

	const char* m_FileLocation;
};

