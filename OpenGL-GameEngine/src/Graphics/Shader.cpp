#include "Shader.h"

Shader::Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation, const char* geometryShaderLocation)
{
	GLuint vertexShader = compileShader(vertexShaderLocation, GL_VERTEX_SHADER);
	GLuint geometryShader = 0;
	GLuint fragmentShader = compileShader(fragmentShaderLocation, GL_FRAGMENT_SHADER);

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);

	if (geometryShaderLocation != nullptr) {
		geometryShader = compileShader(geometryShaderLocation, GL_GEOMETRY_SHADER);
		glAttachShader(shaderID, geometryShader);
	} 

	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);

	int success = 0;
	char infoLog[512] = { "\0" };
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "Error Linking Shaders: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometryShaderLocation != nullptr) glDeleteShader(geometryShader);
}

void Shader::enableShader() {
	glUseProgram(shaderID);
}

void Shader::disableShader() {
	glUseProgram(0);
}

std::string Shader::loadShaderSource(const char* filename) {
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

GLuint Shader::compileShader(const char* filepath, GLenum type)
{
	// compile shader
	GLuint theShader;
	theShader = glCreateShader(type);
	std::string shaderSource = loadShaderSource(filepath);
	const GLchar* shaderCode = shaderSource.c_str();
	glShaderSource(theShader, 1, &shaderCode, NULL);
	glCompileShader(theShader);
	// error handling
	int success = 0;
	char infoLog[512] = { "\0" };
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(theShader, 512, NULL, infoLog);
		std::cout << "Error with " << type << " shader compilation : " << infoLog << std::endl;
	}

	return theShader;
}

// Uniform Setters in shader
void Shader::setUniform(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(shaderID, name.c_str()), 1, &value[0]);
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}


Shader::~Shader()
{
	glDeleteProgram(shaderID);
}
