#pragma once

class Application;

class State {
public:
    State(Application* application) : app(application) {}
    virtual ~State() = default;

    // Called once when the state is added to the stack
    virtual void Initialize() = 0;

    // Called every frame (logic & input)
    virtual void Update(float dt) = 0;

    // Called every frame (graphics)
    virtual void Render() = 0;

    // Stop rendering states below an Opaque state
    virtual bool isOpaque() const { return false; }

protected:
    Application* app;
};