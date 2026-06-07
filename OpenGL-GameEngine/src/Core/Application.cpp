#include "Application.h"
#include <iostream>

#include "Window.h"
#include "Shader.h"
#include "Mesh.h"
#include "Input.h" 
#include "Camera.h"
#include "Object.h"
#include "Material.h"
#include "StandardMeshes.h"

Application::Application() : window(nullptr), coreShader(nullptr), camera(nullptr), lastFrameTime(0.0), frameCount(0), fpsTimer(0.0) {
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

    // Load Textures & Material
    assets.textures["obama"] = new Texture("assets/Textures/obama_sandwich.jpg", "texture_diffuse");
    assets.textures["flag"] = new Texture("assets/Textures/community.png", "texture_diffuse");


    assets.materials["obamaSandwich"] = new Material(assets.textures["obama"], 0);
    assets.materials["communityFlag"] = new Material(assets.textures["flag"], 0);

    // Build Mesh & Object
    assets.meshes["cube"] = StandardMeshes::CreateCube();

    Object* obamaCube = new Object(assets.meshes["cube"], assets.materials["obamaSandwich"]);
    Object* flagCube = new Object(assets.meshes["cube"], assets.materials["communityFlag"]);

    objects.push_back(obamaCube);
    objects.push_back(flagCube);

    // Initial Object Setup
    flagCube->transform.Translate(glm::vec3(0.0f, 2.0f, 0.0f));

    // Set starting time
    lastFrameTime = glfwGetTime();

    return true;
}

// --- Game Logic ---
void Application::Update(double dt) {
    // handles OS-level things (like pressing ESC to close)
    window->processInput();

    // Transform Object
    objects[0]->transform.Rotate(60 * (float)dt, glm::vec3(1.0f, 1.0f, 0.0f));
    objects[1]->transform.Rotate(40 * (float)dt, glm::vec3(0.0f, 0.0f, 1.0f));

    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
}

// --- Rendering Graphics ---
void Application::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    coreShader->enableShader();

    camera->setUniforms(coreShader, window->getWidth(), window->getHeight());

    for (Object* obj : objects) {
        obj->Render(coreShader);
    }

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
    for (Object* obj : objects) { delete obj; }
    objects.clear();
    if (coreShader) { delete coreShader; coreShader = nullptr; }
    if (window) { delete window;     window = nullptr; }
}