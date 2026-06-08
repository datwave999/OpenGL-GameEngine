#include "Application.h"
#include <iostream>

#include "Window.h"
#include "Input.h" 
#include "PlayState.h" // To push initial state

Application::Application() : lastFrameTime(0.0), frameCount(0), fpsTimer(0.0) {
}

Application::~Application() {
    Shutdown(); 
}

// --- Initialization ---
bool Application::Initialize() {

    // 1. Create Window
    window = std::make_unique<Window>(1920, 1080, "OpenGL Game Engine");
    if (window->getNativeWindow() == nullptr) {
        std::cout << "Failed to initialize Window!" << std::endl;
        return false;
    }

    // 2. Global Engine Settings
    glEnable(GL_DEPTH_TEST);
    glfwSwapInterval(0); // VSync (0 = Off, 1 = On)

    // 3. Pass Callbacks to GLFW
    glfwSetKeyCallback(window->getNativeWindow(), Input::keyCallback);
    glfwSetMouseButtonCallback(window->getNativeWindow(), Input::mouseButtonCallback);
    glfwSetCursorPosCallback(window->getNativeWindow(), Input::cursorPosCallback); 
    glfwSetScrollCallback(window->getNativeWindow(), Input::mouseWheelCallback);

    // 4. Set starting time
    lastFrameTime = glfwGetTime();

    // 5. START THE GAME
    PushState(std::make_unique<PlayState>());

    return true;
}

// --- State Management ---
void Application::PushState(std::unique_ptr<State> state) {
    state->Initialize(this);
    states.push_back(std::move(state));
}

void Application::PopState() {
    if (!states.empty()) {
        states.pop_back();
    }
}

// --- Internal Engine Logic ---
void Application::Update(double dt) {
    // OS-level things 
    window->processInput();

    // Top State gets Updated
    if (!states.empty()) {
        states.back()->Update(this, dt);
    }
}

// --- Rendering Graphics ---
void Application::Render() {
    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render all states from bottom to top
    for (const auto& state : states) {
        state->Render(this);
    }
}

// --- Main Engine Loop ---
void Application::Run() {
    while (!window->shouldClose() && !states.empty()) {
        
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
    // Destroy all states
    states.clear(); 
    
    // Clear asset cache (just empty map keys)
    assets.CleanCache();
}