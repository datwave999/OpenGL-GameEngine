#pragma once

#include<glad/glad.h>

#include<iostream>
#include<fstream>
#include<sstream>
#include<streambuf>
#include<string>


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader() = default;
	Shader(const char* vertexShaderLocation, const char* fragmentShaderLocation, const char* geometryShaderLocation = nullptr);
	void enableShader();
	void disableShader();

	// utility functions
	std::string loadShaderSource(const char* filename);
	GLuint compileShader(const char* filepath, GLenum type);

	// uniform functions
	void setUniform(const std::string& name, bool value) const;
	void setUniform(const std::string& name, int value) const;
	void setUniform(const std::string& name, float value) const;
	void setUniform(const std::string& name, const glm::vec3& value) const;
	void setUniform(const std::string& name, const glm::mat4& mat) const;
	void setUniform(const std::string& name, const glm::mat3& mat) const;

	~Shader();

private:
	GLuint shaderID = 0;

	// Caching uniform locations
	mutable std::unordered_map<std::string, GLint> uniformLocationCache;
	GLint GetUniformLocation(const std::string& name) const;
};
