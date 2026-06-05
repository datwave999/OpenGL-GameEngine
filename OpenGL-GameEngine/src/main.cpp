#include<iostream>

#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Shader.h"
#include"Window.h"

GLuint createMesh() {
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


	// VAO, VBO
	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// pass data into VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	// unbinded VAO

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	return VAO;
}

/*
	MAIN FUNCTION ================================================================
*/

int main() {

	//Creating window
	Window window(800, 600, "OpenGL Game Engine");
	if (window.getNativeWindow() == nullptr) return -1;

	//Creating shaders
	Shader coreShader("assets/Shaders/core.vert", "assets/Shaders/core.frag");

	GLuint VAO = createMesh();

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
		glBindVertexArray(VAO);
		coreShader.enableShader();

		model = glm::rotate(model, static_cast<float>(glm::radians(glfwGetTime() / 10.0f)), glm::vec3(0.0f, 0.0f, 1.0f));
		coreShader.setUniform("model", model);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		coreShader.disableShader();

		window.swapBuffers();
		window.pollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	return 0;
}


/*
src/

├── main.cpp

├── Application.cpp

├── Window.cpp

├── rendering/

│   ├── Shader.cpp
		
		Mesh.cpp
		
│   ├── Buffer.cpp

│   ├── VertexArray.cpp

│   └── Texture.cpp

└── scene/

	└── Camera.cpp
*/
