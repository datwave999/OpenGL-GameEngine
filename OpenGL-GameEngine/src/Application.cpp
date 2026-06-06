#include "Application.h"
#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Input.h" 
#include "Camera.h"

Application::Application() : window(nullptr), coreShader(nullptr), cube(nullptr), obama(nullptr), flag(nullptr), camera(nullptr), lastFrameTime(0.0), frameCount(0), fpsTimer(0.0) {
    model = glm::mat4(1.0f);
}

Application::~Application() {
    Shutdown(); 
}

// --- Initialization ---
bool Application::Initialize() {

    // Create Window
    window = new Window(800, 600, "OpenGL Game Engine");
    if (window->getNativeWindow() == nullptr) {
        std::cout << "Failed to initialize Window!" << std::endl;
        return false;
    }

    // --- GLOBAL ENGINE SETTINGS ---
    glEnable(GL_DEPTH_TEST);

        // VSync (0 = Off, 1 = On)
    glfwSwapInterval(0);

    // ------------------------------
    

    // Create Camera
    camera = new Camera();

    //Locks cursor
    glfwSetInputMode(window->getNativeWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Pass Callbacks to GLFW
    glfwSetKeyCallback(window->getNativeWindow(), Input::keyCallback);
    glfwSetMouseButtonCallback(window->getNativeWindow(), Input::mouseButtonCallback);
    glfwSetCursorPosCallback(window->getNativeWindow(), Input::cursorPosCallback); 
    glfwSetScrollCallback(window->getNativeWindow(), Input::mouseWheelCallback);

    // Compile Shaders
    coreShader = new Shader("assets/Shaders/core.vert", "assets/Shaders/core.frag");

    // Mesh Geometry
    // 24 Vertices (4 per face)
// Format: X, Y, Z, U, V
    GLfloat vertices[] = {
        // FRONT FACE (Z = 0.5)
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, // 0: Bottom-left
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // 1: Bottom-right
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // 2: Top-right
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // 3: Top-left

        // BACK FACE (Z = -0.5)
        -0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // 4: Bottom-right (looking from back)
         0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // 5: Bottom-left
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // 6: Top-left
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // 7: Top-right

        // LEFT FACE (X = -0.5)
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // 8: Bottom-left
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, // 9: Bottom-right
        -0.5f,  0.5f,  0.5f,    1.0f, 1.0f, // 10: Top-right
        -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // 11: Top-left

        // RIGHT FACE (X = 0.5)
         0.5f, -0.5f,  0.5f,    0.0f, 0.0f, // 12: Bottom-left
         0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // 13: Bottom-right
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // 14: Top-right
         0.5f,  0.5f,  0.5f,    0.0f, 1.0f, // 15: Top-left

         // TOP FACE (Y = 0.5)
         -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, // 16: Bottom-left
          0.5f,  0.5f,  0.5f,    1.0f, 0.0f, // 17: Bottom-right
          0.5f,  0.5f, -0.5f,    1.0f, 1.0f, // 18: Top-right
         -0.5f,  0.5f, -0.5f,    0.0f, 1.0f, // 19: Top-left

         // BOTTOM FACE (Y = -0.5)
         -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, // 20: Bottom-left
          0.5f, -0.5f, -0.5f,    1.0f, 0.0f, // 21: Bottom-right
          0.5f, -0.5f,  0.5f,    1.0f, 1.0f, // 22: Top-right
         -0.5f, -0.5f,  0.5f,    0.0f, 1.0f  // 23: Top-left
    };

    // 36 Indices (6 faces * 2 triangles per face * 3 vertices per triangle)
    GLuint indices[] = {
        // Front
        0, 1, 2,  2, 3, 0,
        // Back
        4, 5, 6,  6, 7, 4,
        // Left
        8, 9, 10, 10, 11, 8,
        // Right
        12, 13, 14, 14, 15, 12,
        // Top
        16, 17, 18, 18, 19, 16,
        // Bottom
        20, 21, 22, 22, 23, 20
    };

    // Load Textures
    obama = new Texture("assets/Textures/obama_sandwich.jpg", "texture_diffuse");
    flag = new Texture("assets/Textures/community.png", "texture_diffuse");

    // Build Mesh
    cube = new Mesh(vertices, sizeof(vertices), indices, sizeof(indices), flag);

    // Initial Shader Setup


    // Set starting time
    lastFrameTime = glfwGetTime();

    return true;
}

// --- Game Logic ---
void Application::Update(double dt) {
    // handles OS-level things (like pressing ESC to close)
    window->processInput();

    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
}

// --- Rendering Graphics ---
void Application::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    coreShader->enableShader();

    camera->setUniforms(coreShader, window->getWidth(), window->getHeight());

    cube->Render(coreShader, model);

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

        // 2. FPS count
        fpsTimer += dt;
        frameCount++;

        if (fpsTimer >= 1.0) {
            std::string newTitle = "OpenGL Game Engine - FPS: " + std::to_string(frameCount);
            window->setWindowTitle(newTitle.c_str());
            fpsTimer -= 1.0;
            frameCount = 0;
        }

        // 3. Execute Engine Phases
        Update(dt);
        Render();

        // 4. End of Frame Tasks
        Input::EndFrame();     
        window->swapBuffers();
        window->pollEvents();    
    }
}

// --- Memory Cleanup ---
void Application::Shutdown() {
    if (cube) { delete cube;     cube = nullptr; }
    if (obama) { delete obama;      obama = nullptr; }
    if (flag) { delete flag;       flag = nullptr; }
    if (coreShader) { delete coreShader; coreShader = nullptr; }
    if (window) { delete window;     window = nullptr; }
}