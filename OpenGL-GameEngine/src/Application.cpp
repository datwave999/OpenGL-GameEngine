#include "Application.h"
#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Input.h" 

Application::Application()
    : window(nullptr), coreShader(nullptr), square(nullptr), obama(nullptr), flag(nullptr), lastFrameTime(0.0) {
}

Application::~Application() {
    Shutdown(); 
}

// --- Initialization ---
bool Application::Initialize() {
    window = new Window(800, 600, "OpenGL Game Engine");
    if (window->getNativeWindow() == nullptr) {
        std::cout << "Failed to initialize Window!" << std::endl;
        return false;
    }

    // IMPORTANT: Register your unified Input callbacks here!
    glfwSetKeyCallback(window->getNativeWindow(), Input::keyCallback);
    glfwSetMouseButtonCallback(window->getNativeWindow(), Input::mouseButtonCallback);
    glfwSetCursorPosCallback(window->getNativeWindow(), Input::cursorPosCallback); 
    glfwSetScrollCallback(window->getNativeWindow(), Input::mouseWheelCallback);

    // Compile Shaders
    coreShader = new Shader("assets/Shaders/core.vert", "assets/Shaders/core.frag");

    // Mesh Geometry
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
         0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
         0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,    0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 2, 3,
        0, 1, 2
    };

    // Load Textures
    obama = new Texture("assets/Textures/obama_sandwich.jpg", "texture_diffuse");
    flag = new Texture("assets/Textures/community.png", "texture_diffuse");

    // Build Mesh
    square = new Mesh(vertices, sizeof(vertices), indices, sizeof(indices), flag);

    // Initial Shader Setup
    model = glm::mat4(1.0f);
    coreShader->enableShader();
    coreShader->setUniform("texture1", 0);
    coreShader->disableShader();

    // Set starting time
    lastFrameTime = glfwGetTime();

    return true;
}

// --- Game Logic ---
void Application::Update(double dt) {
    // We can keep the window's generic input process if it handles OS-level things (like pressing ESC to close)
    window->processInput();

    // --- TEST YOUR NEW INPUT SYSTEM ---
    // Let's use Delta Time (dt) to move the square at a consistent speed!

    float speed = 2.0f; // Move 2 units per second

    if (Input::getKeyState(GLFW_KEY_W)) {
        model = glm::translate(model, glm::vec3(0.0f, speed * dt, 0.0f)); // Move Up
    }
    if (Input::getKeyState(GLFW_KEY_S)) {
        model = glm::translate(model, glm::vec3(0.0f, -speed * dt, 0.0f)); // Move Down
    }

    // Test the "Single Press" edge detection
    if (Input::keyWentDown(GLFW_KEY_SPACE)) {
        std::cout << "Spacebar hit exactly once!" << std::endl;
    }
}

// --- Rendering Graphics ---
void Application::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    coreShader->enableShader();
    coreShader->setUniform("model", model);

    square->Render();

    coreShader->disableShader();
}

// --- Main Engine Loop ---
void Application::Run() {
    while (!window->shouldClose()) {
        // 1. Calculate Delta Time 
        double currentFrameTime = glfwGetTime();
        double dt = currentFrameTime - lastFrameTime;
        if (dt > 0.1) dt = 0.1; // Cap at 100ms
        lastFrameTime = currentFrameTime;

        // 2. Execute Engine Phases
        Update(dt);
        Render();

        // 3. End of Frame Tasks
        Input::EndFrame();     
        window->swapBuffers();
        window->pollEvents();    
    }
}

// --- Memory Cleanup ---
void Application::Shutdown() {
    if (square) { delete square;     square = nullptr; }
    if (obama) { delete obama;      obama = nullptr; }
    if (flag) { delete flag;       flag = nullptr; }
    if (coreShader) { delete coreShader; coreShader = nullptr; }
    if (window) { delete window;     window = nullptr; }
}