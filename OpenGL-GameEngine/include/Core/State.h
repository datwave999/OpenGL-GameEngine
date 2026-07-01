#pragma once

class Application;

class State {
public:
    virtual ~State() = default;

    // Called once when the state is added to the stack
    virtual void Initialize(Application* app) = 0;

    // Called every frame (logic & input)
    virtual void Update(Application* app, float dt) = 0;

    // Called every frame (graphics)
    virtual void Render(Application* app) = 0;

    // Stop rendering states below an Opaque state
    virtual bool isOpaque() const { return false; }
};