#pragma once

#include "State.h"
#include <memory>

// Forward Declarations
class Shader;
class Mesh;

class PauseState : public State {
public:
    PauseState();
    ~PauseState() override;

    void Initialize(Application* app) override;
    void Update(Application* app, double dt) override;
    void Render(Application* app) override;
    bool isOpaque() const override { return false; }

private:
    std::shared_ptr<Shader> overlayShader;
    std::shared_ptr<Mesh> overlayMesh;
};