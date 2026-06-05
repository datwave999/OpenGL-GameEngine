#include<iostream>

#include<vector>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Window.h"
#include"Mesh.h"

/*
	MAIN FUNCTION ================================================================
*/

int main() {

	//Creating window
	Window window(800, 600, "OpenGL Game Engine");
	if (window.getNativeWindow() == nullptr) return -1;

	//Creating shaders
	Shader coreShader("assets/Shaders/core.vert", "assets/Shaders/core.frag");


	//Mesh
	// vertex array
	GLfloat vertices[] = {           // tex
		-0.5f, -0.5f, 0.0f,			0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,			1.0f, 0.0f,
		0.5f, 0.5f, 0.0f,			1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f,			0.0f, 1.0f
	};

	// Index array
	GLuint indices[] = {
		0, 2, 3,
		0, 1, 2
	};

	Texture* obama = new Texture("assets/Textures/obama_sandwich.jpg", "texture_diffuse");
	Texture* flag = new Texture("assets/Textures/community.png", "texture_diffuse");

	Mesh square(vertices, sizeof(vertices), indices, sizeof(indices), flag);

	//Uniforms
	glm::mat4 model(1.0f);
	coreShader.enableShader();
	coreShader.setUniform("texture1", 0);
	coreShader.disableShader();

	/*
	=======================
		MAIN GAME LOOP
	=======================
	*/
	while (!window.shouldClose()) {
		// process input
		window.processInput();

		// render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Shapes
		coreShader.enableShader();

		//model = glm::rotate(model, static_cast<float>(glm::radians(glfwGetTime() / 10.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		coreShader.setUniform("model", model);

		square.Render();

		coreShader.disableShader();

		window.swapBuffers();
		window.pollEvents();
	}

	delete obama;
	return 0;
}

