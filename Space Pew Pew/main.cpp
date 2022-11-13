#define STB_IMAGE_IMPLEMENTATION

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Shader.h"
#include "Rectangle.h"
#include "Object.h"
#include "Texture.h"
#include "Starship.h"
#include "EnemyStarship.h"
#include "Game.h"

#include <memory>
#include <map>

#include <irrKlang.h>

#include "TextRenderer.h"

int main()
{
	srand(time(NULL));
	// Delta time
	GLfloat now;
	GLfloat deltaTime = 0.0f;
	GLfloat lastTime = 0.0f;

	Window window;

	// Window
	window = Window(640, 800);
	window.CreateNewWindow();

	// Shaders
	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(new Shader());
	shader->CreateFromFiles("Shaders/shader.vert", "Shaders/shader.frag");
	shader->UseShader();
	shader->Validate();

	std::shared_ptr<Shader> textShader = std::shared_ptr<Shader>(new Shader());
	textShader->CreateFromFiles("Shaders/textShader.vert", "Shaders/textShader.frag");
	shader->UseShader();
	shader->Validate();

	// Projection matrix uniform
	glm::mat4 projection = glm::ortho((GLfloat)(-(window.GetBufferWidth() / 2)), (GLfloat)(window.GetBufferWidth() / 2),
		(GLfloat)(-(window.GetBufferHeight() / 2)), (GLfloat)(window.GetBufferHeight() / 2));

	GLuint uProjection = shader->GetProjectionLocation();

	glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

	// Get model matrix location
	GLuint uModel = shader->GetModelLocation();

	now = glfwGetTime();
	deltaTime = now - lastTime;
	lastTime = now;

	stbi_set_flip_vertically_on_load(true);

	std::shared_ptr<Rectangle> rect(new Rectangle());

	Game game(rect, uModel, shader->GetShaderID(), window.GetKeys(), window.GetWidth(), window.GetHeight(), shader, textShader);

	// Reset keys
	window.GetKeys()[GLFW_KEY_W] = false;
	window.GetKeys()[GLFW_KEY_S] = false;
	window.GetKeys()[GLFW_KEY_A] = false;
	window.GetKeys()[GLFW_KEY_D] = false;
	window.GetKeys()[GLFW_KEY_SPACE] = false;
	window.GetKeys()[GLFW_KEY_T] = false;

	while (!window.GetShouldClose())
	{
		now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		glfwPollEvents();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		game.Update(deltaTime);

		window.SwapBuffers();
	}
}