#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

#include "Texture.h"

class Window
{
public:
	Window();
	Window(unsigned int width, unsigned int height);

	bool CreateNewWindow();

	bool GetShouldClose() { return glfwWindowShouldClose(m_Window); }
	int GetBufferWidth() { return m_BufferWidth; }
	int GetBufferHeight() { return m_BufferHeight; }
	int GetWidth() { return m_Width;}
	int GetHeight() { return m_Height; }
	bool* GetKeys() { return m_Keys; }

	void SwapBuffers() { glfwSwapBuffers(m_Window); }

	~Window();

private:
	GLint m_Width, m_Height;
	GLint m_BufferWidth, m_BufferHeight;

	bool m_Keys[1024];

	GLFWwindow* m_Window;

	void CreateCallback();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
};

