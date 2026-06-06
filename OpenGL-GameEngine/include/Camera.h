#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Forward Declaration for Shader*
class Shader;

class Camera
{
public:
	Camera(glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 3.0f));

	void Update(double dt);

	// Delegation
	void setUniforms(Shader* shader, int screenWidth, int screenHeight) const;

    // --- GETTERS ---
    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    float getFOV() const;

    // Matrix Generators
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float screenWidth, float screenHeight) const;

    // --- SETTERS & MODIFIERS ---
    void setMovementSpeed(float speed);
    void processMouseScroll(double yOffset);

private:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler Angles
    float Yaw;
    float Pitch;

    // Camera Settings
    float MovementSpeed;
    float MouseSensitivity;
    float FOV;

    // Internal Math
    void updateCameraVectors();
};

