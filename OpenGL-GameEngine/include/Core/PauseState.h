#pragma once

#include "Core/State.h"
#include <cstdint>

class PauseState : public State {
public:
    PauseState(Application* app);
    ~PauseState() override;

    void Initialize() override;
    void Update(float dt) override;
    void Render() override;
    bool isOpaque() const override { return false; }

private:
    uint32_t overlayShaderHandle;
    uint32_t overlayMeshHandle;
};