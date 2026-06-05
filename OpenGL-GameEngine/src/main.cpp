#include<iostream>

#include<vector>

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
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	// Index array
	GLuint indices[] = {
		0, 2, 3,
		0, 1, 2
	};

	Mesh square(vertices, sizeof(vertices), indices, sizeof(indices));

	//Uniforms
	glm::mat4 model(1.0f);


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

		model = glm::rotate(model, static_cast<float>(glm::radians(glfwGetTime() / 10.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		coreShader.setUniform("model", model);

		square.Render();

		coreShader.disableShader();

		window.swapBuffers();
		window.pollEvents();
	}

	
	return 0;
}

