#include "Window.h"

Window::Window()
{
	m_Width = 1024, m_Height = 720;
	m_BufferWidth = 0, m_BufferHeight = 0;
}

Window::Window(unsigned int width, unsigned int height)
{
	m_Width = width, m_Height = height;
	m_BufferWidth = 0, m_BufferHeight = 0;
}

bool Window::CreateNewWindow()
{
	// Init glfw
	if (!glfwInit())
	{
		std::cout << "GLFW INIT FAILED." << std::endl;
		glfwTerminate();
		return false;
	}

	// Setup GLFW Window hints
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glewExperimental = GL_TRUE;

	m_Window = glfwCreateWindow(m_Width, m_Height, "WarSpace", nullptr, nullptr);

	if (!m_Window)
	{
		std::cout << "Error creating GLFW window!" << std::endl;
		glfwTerminate();
		return false;
	}

	// Center window
	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_Window, (videoMode->width - m_Width) / 2, (videoMode->height - m_Height) / 2);

	// Get buffer size information
	glfwGetFramebufferSize(m_Window, &m_BufferWidth, &m_BufferHeight);

	// Set the current context
	glfwMakeContextCurrent(m_Window);
	
	// Create callbacks
	CreateCallback();

	// Create icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("Textures/icon.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
	glfwSetWindowIcon(m_Window, 1, images);
	stbi_image_free(images[0].pixels);

	// Init glew
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return false;
	}

	glViewport(0, 0, m_BufferWidth, m_BufferHeight);

	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetWindowUserPointer(m_Window, this);
}

void Window::CreateCallback()
{
	glfwSetKeyCallback(m_Window, handleKeys);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->m_Keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->m_Keys[key] = false;
		}
	}
}



Window::~Window()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}