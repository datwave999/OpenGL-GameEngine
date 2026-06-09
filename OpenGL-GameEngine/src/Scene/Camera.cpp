#include "Camera.h"

#include "Input.h" 
#include "Shader.h"

Camera::Camera(glm::vec3 position) {
    Position = position;
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

    Yaw = -90.0f;
    Pitch = 0.0f;

    MovementSpeed = 5.0f;
    MouseSensitivity = 0.05f;
    FOV = 60.0f;

    updateCameraVectors();
}

void Camera::Update(double dt) {
    // --- 1. KEYBOARD MOVEMENT ---
    float velocity = MovementSpeed * static_cast<float>(dt);

    // TEMP WAY TO SPEED UP
    if (Input::keyWentDown(GLFW_KEY_LEFT_SHIFT)) MovementSpeed *= 3.0f;
    if (Input::keyWentUp(GLFW_KEY_LEFT_SHIFT)) MovementSpeed /= 3.0f;

    if (Input::getKeyState(GLFW_KEY_W)) Position += Front * velocity;
    if (Input::getKeyState(GLFW_KEY_S)) Position -= Front * velocity;
    if (Input::getKeyState(GLFW_KEY_A)) Position -= Right * velocity;
    if (Input::getKeyState(GLFW_KEY_D)) Position += Right * velocity;

    // Vertical movement (Space to go up, Left Ctrl to go down)
    if (Input::getKeyState(GLFW_KEY_SPACE)) Position += WorldUp * velocity;
    if (Input::getKeyState(GLFW_KEY_LEFT_CONTROL)) Position -= WorldUp * velocity;

    // --- 2. MOUSE ROTATION ---
    double dx = Input::getDX();
    double dy = Input::getDY();

    if (dx != 0.0 || dy != 0.0) {
        Yaw += static_cast<float>(dx) * MouseSensitivity;
        Pitch += static_cast<float>(dy) * MouseSensitivity;

        // Constrain Pitch to avoid Gimbal Lock (flipping upside down)
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;

        updateCameraVectors();
    }
}

// --- DELEGATION ---
void Camera::setUniforms(Shader* shader, int screenWidth, int screenHeight) const {
    shader->setUniform(Uniform::view, getViewMatrix());
    shader->setUniform(Uniform::projection, getProjectionMatrix(static_cast<float>(screenWidth), static_cast<float>(screenHeight)));
}

// --- GETTERS ---
glm::vec3 Camera::getPosition() const { return Position; }
glm::vec3 Camera::getFront() const { return Front; }
float Camera::getFOV() const { return FOV; }

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::getProjectionMatrix(float screenWidth, float screenHeight) const {
    return glm::perspective(glm::radians(FOV), screenWidth / screenHeight, 0.1f, 100.0f);
}

// --- SETTERS & MODIFIERS ---
void Camera::setMovementSpeed(float speed) {
    if (speed < 0.0f) speed = 0.0f;
    MovementSpeed = speed;
}

void Camera::processMouseScroll(double yOffset) {
    FOV -= static_cast<float>(yOffset) * 2.0f;

    if (FOV < 1.0f) FOV = 1.0f;
    if (FOV > 90.0f) FOV = 90.0f;
}

// --- INTERNAL MATH ---
void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Cross product generates the perpendicular vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}