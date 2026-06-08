#pragma once

class Application;

class State {
public:
    virtual ~State() = default;

    // Called once when the state is added to the stack
    virtual void Initialize(Application* app) = 0;

    // Called every frame (logic & input)
    virtual void Update(Application* app, double dt) = 0;

    // Called every frame (graphics)
    virtual void Render(Application* app) = 0;
};