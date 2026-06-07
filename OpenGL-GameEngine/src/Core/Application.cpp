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

Application::Application() : coreShader(nullptr), lastFrameTime(0.0), frameCount(0), fpsTimer(0.0) {
}

Application::~Application() {
    Shutdown(); 
}

// --- Initialization ---
bool Application::Initialize() {

    // Create Window
    window = std::make_unique<Window>(800, 600, "OpenGL Game Engine");
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
    camera = std::make_unique<Camera>();

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
    assets.textures["obama"] = std::make_shared<Texture>("assets/Textures/obama_sandwich.jpg", "texture_diffuse");
    assets.textures["flag"] = std::make_shared<Texture>("assets/Textures/community.png", "texture_diffuse");

    assets.materials["obamaSandwich"] = std::make_shared<Material>(assets.textures["obama"], 0);
    assets.materials["communityFlag"] = std::make_shared<Material>(assets.textures["flag"], 0);

    // Build Mesh & Model
    assets.meshes["cube"] = StandardMeshes::CreateCube();
    assets.meshes["sphere"] = StandardMeshes::CreateSphere();

    assets.models["obamaCube"] = std::make_shared<Model>(assets.meshes["cube"], assets.materials["obamaSandwich"]);
    assets.models["flagCube"] = std::make_shared<Model>(assets.meshes["cube"], assets.materials["communityFlag"]);
    assets.models["sandwichSphere"] = std::make_shared<Model>(assets.meshes["sphere"], assets.materials["obamaSandwich"]);

    // Create Objects
    objects.push_back(std::make_unique<Object>(assets.models["obamaCube"]));
    objects.push_back(std::make_unique<Object>(assets.models["flagCube"]));
    objects.push_back(std::make_unique<Object>(assets.models["sandwichSphere"]));

    // Initial Object Setup
    objects[1]->transform.Translate(glm::vec3(0.0f, 2.0f, 0.0f));
    objects[2]->transform.Translate(glm::vec3(0.0f, 0.0f, 3.0f));

    // Set starting time
    lastFrameTime = glfwGetTime();

    return true;
}

// --- Game Logic ---
void Application::Update(double dt) {
    // handles OS-level things (like pressing ESC to close)
    window->processInput();

    // Transform Object
    objects[1]->transform.Rotate(40 * (float)dt, glm::vec3(0.0f, 0.0f, 1.0f));
    objects[2]->transform.Rotate(50 * (float)dt, glm::vec3(0.0f, 1.0f, 0.0f));

    camera->Update(dt);
    camera->processMouseScroll(Input::getScrollDY());
}

// --- Rendering Graphics ---
void Application::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    coreShader->enableShader();

    camera->setUniforms(coreShader, window->getWidth(), window->getHeight());

    for (const auto& obj : objects) {
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
    objects.clear();
    if (coreShader != nullptr) { delete coreShader; coreShader = nullptr; }
}