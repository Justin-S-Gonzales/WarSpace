#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>
#include <map>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Shader.h"

class TextRenderer
{
public:
	TextRenderer() {}

	TextRenderer(const char* font);

	void RenderText(Shader& s, std::string text, float x, float y, float scale, glm::vec3 color, float width, float height);

private:
	unsigned int m_VAO, m_VBO;

	struct Character {
		unsigned int TextureID;  // ID handle of the glyph texture
		glm::ivec2   Size;       // Size of glyph
		glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
		unsigned int Advance;    // Offset to advance to next glyph
	};

	std::map<char, Character> m_Characters;

};

