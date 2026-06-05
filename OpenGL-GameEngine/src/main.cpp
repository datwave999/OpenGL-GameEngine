#include<iostream>

#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSource(const char* filename);

int main() {

	int success;
	char infoLog[512];


	glfwInit();
	// OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Compatibility with MacOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Creating window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Game Engine", NULL, NULL);
	if (window == nullptr) { 
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//

	/*
		shaders
	*/

	// compile vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderSource = loadShaderSource("assets/Shaders/core.vert");
	const GLchar* vertexShaderCode = vertexShaderSource.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexShader);
		// error handling
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "Error with vertex shader compilation: " << infoLog << std::endl;
	}

	// compile fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderSource = loadShaderSource("assets/Shaders/core.frag");
	const GLchar* fragmentShaderCode = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentShader);
		// error handling
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "Error with fragment shader compilation: " << infoLog << std::endl;
	}

	// Linking to shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
		// error handling
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error Linking Shaders: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

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
	GLuint VAO, VBO , EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// bind VAO
	glBindVertexArray(VAO);

	// bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	// bind EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

	// pass data into VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	// unbinded VAO
	
	// Main Window Loop
	while (!glfwWindowShouldClose(window)) {
		// process input
		processInput(window);

		// render 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw Shapes
		glBindVertexArray(VAO);
		glUseProgram(shaderProgram);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glUseProgram(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// add comment to test git stash

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

std::string loadShaderSource(const char* filename) {
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	file.open(filename);

	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open: " << filename << std::endl;
	}

	file.close();

	return ret;
}