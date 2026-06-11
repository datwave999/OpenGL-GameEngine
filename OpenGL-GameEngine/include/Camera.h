#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>  
#include "Buffer.h"

struct CameraMatrices {
    glm::mat4 projection;
    glm::mat4 view;
};

class Camera
{
public:
	Camera(glm::vec3 initialPosition = glm::vec3(0.0f, 0.0f, 3.0f));

	void Update(double dt);

	// Delegation
    void InitUBO();
    void UpdateUBO(int screenWidth, int screenHeight);

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

    // Camera uniforms
    std::unique_ptr<Buffer> cameraUBO;
};

