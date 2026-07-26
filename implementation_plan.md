# DOD/ECS Architecture Transition Plan (Final)

## Decisions

| Question | Answer |
|----------|--------|
| **Approach** | Phased — slow and steady. Each phase compiles and runs before the next begins. |
| **ECS Library** | [EnTT](https://github.com/skypjack/entt) — header-only, dropped into `Linking/include/`. No custom registry. |
| **AssetContainer** | Gets nuked. Replaced by `AssetStore` with flat `std::vector` storage and `uint32_t` handles. |
| **Scene data** | C++ code for now. JSON scene files are a post-migration follow-up. |

---

## Current Architecture Analysis

### What Stays (Pragmatic OOP Exceptions)

| File | Role | Verdict |
|------|------|---------|
| Window.h | GLFW window wrapper | ✅ Keep |
| Camera.h | FPS camera + UBO | ✅ Keep |
| Input.h | Static input polling | ✅ Keep |
| Shader.h | OpenGL shader program | ✅ Keep |
| Buffer.h | OpenGL buffer object | ✅ Keep |
| VertexArray.h | OpenGL VAO | ✅ Keep |
| Application.h | App shell + state stack | ✅ Keep |
| State.h | State interface | ✅ Keep |
| GeometryUtils.hpp | Normal calculation helpers | ✅ Keep (update include) |
| StandardMeshes.h | Cube/Sphere/Plane generators | ✅ Keep (return type changes) |

### What Gets Replaced

| Current | Replacement |
|---------|-------------|
| Object | `entt::entity` + `TransformComponent` + `RenderComponent` |
| LightObject | `PointLightComponent` / `SpotLightComponent` + `LightVisualizerTag` on entities |
| Transform | POD `TransformComponent` + `TransformSystem` free functions |
| Model | `ModelAsset` POD in `AssetStore`. Drawing in `RenderSystem` |
| Mesh | `MeshGPU` POD (VAO ID + index count). No methods |
| Material | `MaterialAsset` POD. No methods |
| Texture | `TextureGPU` POD (`GLuint`). Loading in `AssetStore`. Binding in `RenderSystem` |
| AssetContainer | New `AssetStore` — flat vectors + `uint32_t` handles |
| LightManager | `LightSystem` querying EnTT views |
| DirectionalLight, PointLight, SpotLight, PositionalLight | Deleted. POD structs from LightData.h become ECS components directly |

---

## Phase 1: EnTT + Components + Asset Store

> Purely additive. No existing files modified. Old code continues to compile and run.

### Phase 1 Execution Checklist (Where we are)
- [x] **Step 1:** Download EnTT to `Linking/include/entt/entt.hpp`
- [x] **Step 2:** Create `OpenGL-GameEngine/include/ECS/Entity.h` (Thin wrapper)
- [x] **Step 3:** Create the domain-specific component headers (`TransformComponent.h`, `RenderComponent.h`, `LightComponents.h`) in `OpenGL-GameEngine/include/ECS/Components/`
- [x] **Step 4:** Create `OpenGL-GameEngine/include/Resources/AssetData.h` (containing Mesh, Texture, Material, Model PODs)
- [x] **Step 5:** Create the new `AssetManager.h` and `AssetManager.cpp` in `OpenGL-GameEngine/include/Resources/` and `OpenGL-GameEngine/src/Resources/`
- [x] **Step 6:** Refactor `AssetData.h` and `AssetManager.h/.cpp` to make `MeshGPU` and `MaterialAsset` strict PODs (replace `unique_ptr` with raw `GLuint` or handle vectors), adhering strictly to the DOD skill rules.

#### EnTT Integration

- Download EnTT single-header from [GitHub releases](https://github.com/skypjack/entt)
- Place at `Linking/include/entt/entt.hpp`
- That's it — header-only, zero build config

#### [NEW] Entity Wrapper — `OpenGL-GameEngine/include/ECS/Entity.h`

*(Implemented)* — Lightweight handle that delegates to `entt::registry` for scene initialization.

#### [NEW] Component Definitions — `OpenGL-GameEngine/include/ECS/Components/`

*(Implemented)* — Contains `TransformComponent`, `RenderComponent`, and `LightComponents` (which includes the UBO structures exactly matching GPU layout). No umbrella header is used.


#### [NEW] AssetData.h — `OpenGL-GameEngine/include/Resources/AssetData.h`

*(Implemented)* — Contains the core DOD structs (`Vertex`, `MeshGPU`, `TextureGPU`, `MaterialAsset`, `ModelAsset`) that break down heavy OOP objects into pure data handles.

#### [NEW] AssetManager.h/.cpp — `OpenGL-GameEngine/include/Resources/AssetManager.h` + `OpenGL-GameEngine/src/Resources/AssetManager.cpp`

*(Implemented)* — Replaces `AssetContainer`. Manages all GPU uploads and implements flat vector storage with `uint32_t` handles.

#### [MODIFY] Step 6: Strict DOD Refactor for Assets

*(Implemented)* — Since `MeshGPU` and `MaterialAsset` must be pure PODs, we will decouple the RAII wrappers (`VertexArray`, `Buffer`) from the asset structs. The wrappers will live in `AssetManager` to handle lifetime (deletion), and the assets will only hold the raw `GLuint` IDs for rendering.

---

## Phase 1.5: Bug Fixes (From Review) - COMPLETED

The following fixes have been successfully applied to the codebase to address compilation and data issues from Phase 1:

#### [MODIFY] `AssetManager.cpp`
- [x] **Bug 2 (Fixed):** `LoadModel` returns `0` on failure. Change `return 0;` to `return UINT32_MAX;` (or `(uint32_t)-1`).
- [x] **Bug 3 (Addressed):** `processMesh` generates overlapping mesh keys. Mitigated by architectural rule (one model per directory ensures uniqueness).
- [x] **Bug 4 (Fixed):** Defensive check for `stbi_image_free`. Wrap it: `if (data) stbi_image_free(data);`.

#### [MODIFY] `TransformComponent.h` & `TransformSystem.cpp`
- [x] **Warning 1 (Fixed):** Remove `#define GLM_ENABLE_EXPERIMENTAL` from `TransformComponent.h` and move it to the top of `TransformSystem.cpp`.

#### [MODIFY] `StandardMeshes` and `GeometryUtils` (Bug 1) - [x] COMPLETED
- Changed include from `deprecated/Mesh.h` to `Resources/AssetData.h`.
- Stripped OOP `Mesh` generation. Functions now return raw `MeshData` (`std::pair<std::vector<Vertex>, std::vector<GLuint>>`) for the `AssetManager`.

---

## Phase 2: Systems (Implementation Details) - [x] COMPLETED

> Purely additive phase. Three new DOD systems have been implemented that act purely on EnTT registries and Asset handles.

### 1. TransformSystem [x]
**Path:** `OpenGL-GameEngine/include/ECS/Systems/TransformSystem.h` and `src/ECS/Systems/TransformSystem.cpp`
Replaced the old OOP `Transform` class with free-standing functions to mutate `TransformComponent` data. The batch update loop safely caches final Model and Normal matrices ONLY when a transform is dirty.

### 2. LightSystem [x]
**Path:** `OpenGL-GameEngine/include/ECS/Systems/LightSystem.h` and `src/ECS/Systems/LightSystem.cpp`
Replaced OOP Lights with `PointLightComponent` and `SpotLightComponent`. 
- Implements `LightOffsetComponent` math natively for parent-less attachment.
- Pre-calculates squared distance and uses `std::partial_sort` into class-member caches to avoid heap allocations and full array sorting.
- Uploads the closest N lights to the global `LightUBO`.

### 3. RenderSystem [x]
**Path:** `OpenGL-GameEngine/include/ECS/Systems/RenderSystem.h` and `src/ECS/Systems/RenderSystem.cpp`
Replaced hierarchical loop rendering with a purely data-driven flat array Render Queue.
- Extracts lightweight `RenderCommand`s from entities.
- Sorts primarily by `shaderHandle` and secondarily by `materialHandle` to batch OpenGL state changes.
- Uses cached `normalMatrix` from the `TransformSystem`.
- Tracks `currentMesh` to prevent redundant `glBindVertexArray` calls for identical models.

---

## Phase 3: Rewire Application & PlayState (The Integration) - [x] COMPLETED

> The integration phase. Deprecated OOP classes stop being used. The scene is rebuilt purely with DOD structs.

- **Application Core:** Swapped `AssetContainer` for the new `AssetManager`. 
- **PlayState Headers:** Removed all OOP object arrays and introduced `entt::registry`, `TransformSystem`, `LightSystem`, and `RenderSystem`.
- **PlayState Logic:** 
  - Spawned `Entity` instances with components (e.g., `TransformComponent`, `RenderComponent`) instead of `Object` instances.
  - Replaced the OOP `Update()` hot loops with clean calls to `transformSystem.Update(registry)` and `lightSystem.Update(...)`.
  - Replaced the OOP `Render()` loop with a single `renderSystem.Render(...)` call.
- **Mesh Utils:** Updated `StandardMeshes` and `GeometryUtils` to return raw `MeshData` arrays instead of `std::shared_ptr<Mesh>`.

---

## Phase 3 Bug Fixes: Decoupling Light Data (Two-Struct Approach) - [x] COMPLETED

> Implemented to preserve clean component initialization in the ECS while ensuring strict `std140` padding for the GPU UBOs.

- **LightComponents:** Separated `PointLightComponent` (game data) from `PointLightUBOData` (padded GPU data).
- **LightSystem:** Added math inside the `Update` loop to convert `radius` to attenuation factors (`constant`, `linear`, `quadratic`) and map the ECS components directly to the GPU structs prior to uploading.
- **PlayState:** Updated light spawning to use the clean component parameters.

---

## Phase 4: Cleanup - [x] COMPLETED

- **Deleted Deprecated Code:** Removed `src/deprecated/` and `include/deprecated/` entirely (26 files).
- **Cleaned PlayState.h:** Removed old OOP forward declarations (`Object`, `LightObject`).
- **Updated vcxproj:** Visual Studio automatically updated the project files to reflect the deletions.

---

## Architecture After Transition

```
Core (OOP — Kept)          ECS (DOD — EnTT)              Assets (New)
─────────────────          ────────────────               ────────────
Application                entt::registry                AssetManager
Window                     TransformComponent              → ModelAsset
Camera                     RenderComponent                  → MeshGPU
Input                      PointLightComponent              → MaterialAsset
State Stack                SpotLightComponent               → TextureGPU
                           LightVisualizerTag
Graphics (OOP — Kept)
─────────────────          TransformSystem
Shader                     LightSystem
Buffer                     RenderSystem
VertexArray
```

---

## Verification Plan

### Per-Phase Build Check
- After each phase, build from Visual Studio to confirm zero errors/warnings

### Visual Verification (After Phase 3)
The rendered scene must be **pixel-identical** to the current OOP version:
- All 7 objects at correct positions (obama cube, flag cube, sphere, sedan, race, floor, backpack)
- Flag cube rotating on Z, sphere rotating on Y, race car moving forward + rotating
- 3 point lights (blue orbiting, red static, green static) + 1 yellow spotlight
- Directional sun light
- Camera WASD + mouse unchanged
- Pause state (ESC) functional

### Post-Cleanup (After Phase 4)
- `grep -r "deprecated" OpenGL-GameEngine/include/ OpenGL-GameEngine/src/` returns zero hits
- No `shared_ptr` or `unique_ptr` in any ECS component, asset, or system hot path
- FPS counter >= previous (better cache locality)

---

## Phase 5: Future Architectural Migrations (Post-Transition)

Once the core DOD/ECS transition is complete and stable, the following features are recommended to fully mature the engine architecture:

### 1. Dynamic Asset Unloading (Reference Counting via EnTT)

To support Open World streaming and automated level unloading, we will implement an automated Garbage Collector inside `AssetManager` that hooks directly into the ECS.

1. Every asset struct gets a `uint32_t keyHash` (to erase it from the cache) and a `uint32_t refCount`. We use integer hashes instead of `std::string` to guarantee zero micro-stutters during dynamic mid-game open-world streaming.
2. We hook into EnTT's events: whenever a `RenderComponent` is added to or removed from an entity, EnTT will automatically notify the `AssetManager` to increase or decrease the `refCount`.
3. The `GarbageCollect()` function sweeps the arrays. If an asset has 0 references, it deletes the OpenGL memory, erases the map key, and adds the array index to a **Free-List** so the next loaded asset can reuse that slot.

#### Proposed Code Changes

##### 1. Add Fast Hashing & Update Asset PODs (`include/Resources/AssetData.h`)
We will add a fast constexpr hash function (like FNV-1a) to avoid `std::string` allocations, and update the structs:
```cpp
// Fast string hasher
constexpr uint32_t HashString(const char* str) {
    uint32_t hash = 2166136261u;
    for (int i = 0; str[i] != '\0'; ++i) {
        hash ^= static_cast<uint32_t>(str[i]);
        hash *= 16777619u;
    }
    return hash;
}

struct TextureGPU {
    GLuint textureID = 0;
    uint32_t keyHash = 0;
    uint32_t refCount = 0;
};
// (Same for MeshGPU, MaterialAsset, ModelAsset)
```

##### 2. Update AssetManager (`include/Resources/AssetManager.h`)
We will swap the maps to use purely `uint32_t`, making them blazingly fast:
```cpp
class AssetManager {
public:
    // ... existing loaders ...
    
    // Reference Counting (Called automatically by EnTT)
    void AddRef(uint32_t modelHandle);
    void ReleaseRef(uint32_t modelHandle);
    void GarbageCollect();

private:
    // Caching map (Hash -> Handle) - Zero String Allocations!
    std::unordered_map<uint32_t, uint32_t> textureMap;
    // ... (same for other maps) ...

    std::vector<uint32_t> freeTextures;
    // ... (same for other free-lists) ...
};
```

##### 3. Connect EnTT Signals (`src/Core/PlayState.cpp`)
In `PlayState::Initialize()`, we just tell EnTT to notify the `AssetManager` whenever a component is created or destroyed. We never have to manually track memory again!
```cpp
// When a RenderComponent is assigned to an entity, auto-increment the ref count
registry.on_construct<RenderComponent>().connect<&AssetManager::OnComponentAdded>(assets);

// When an entity is destroyed (or the level unloads), auto-decrement the ref count
registry.on_destroy<RenderComponent>().connect<&AssetManager::OnComponentRemoved>(assets);
```

##### 4. The Garbage Collector & Loader Allocation
The Garbage Collector deletes using the integer hash, eliminating all string bottlenecks.
```cpp
void AssetManager::GarbageCollect() {
    for (uint32_t i = 0; i < textures.size(); i++) {
        if (i == defaultTextureHandle || textures[i].textureID == 0) continue; 
        
        if (textures[i].refCount == 0) {
            glDeleteTextures(1, &textures[i].textureID); // Free GPU
            textures[i].textureID = 0;                   // Mark as empty
            textureMap.erase(textures[i].keyHash);       // Clear from cache instantly!
            freeTextures.push_back(i);                   // Add to free-list
        }
    }
}
```

### 2. ECS Camera System
Currently, the `Camera` remains a standalone OOP class that manages its own UBO and logic.
- **Goal:** Decouple camera data from its logic and GPU resources.
- **Plan:**
  - Create a `CameraComponent` (POD) storing FOV, near/far planes, and an `isActive` flag.
  - Let the entity's `TransformComponent` dictate the Camera's position, pitch, and yaw.
  - Create a `CameraSystem` that queries for the active `CameraComponent` + `TransformComponent`, calculates the View/Projection matrices, and handles the `cameraUBO` upload.
- **Benefit:** Seamlessly switch between multiple cameras in a scene without instantiating/destroying heavyweight objects.

### 3. Scene Graph (Transform Hierarchy)
Currently, all `TransformComponent` matrices are global. The engine lacks a formal Parent-Child attachment system.
- **Goal:** Support hierarchical attachments (e.g., a flashlight attached to a player's hand).
- **Plan:**
  - Introduce a `RelationshipComponent` storing `entt::entity parent`, `first_child`, `prev_sibling`, and `next_sibling`.
  - Update `TransformSystem` to perform a topological sort or hierarchical traversal. A child's final `modelMatrix` is computed by multiplying its local transform by its parent's global `modelMatrix`.
- **Benefit:** Eliminates the need for hacky one-off offset components (like `LightOffsetComponent`) and standardizes all object attachments.

### 4. CPU Frustum Culling
Currently, all entities with a `RenderComponent` are pushed to the `RenderQueue` regardless of whether they are on-screen, forcing the GPU pipeline to perform the clipping.
- **Goal:** Discard off-screen entities on the CPU before issuing OpenGL draw calls to save CPU time and PCIe bandwidth.
- **Plan:**
  - Add bounding volumes (AABB or Bounding Sphere) to the `ModelAsset` during the Assimp loading phase.
  - Update the `RenderSystem` extraction loop to check if the entity's bounding volume intersects with the Camera's frustum planes.
  - If the entity is outside the frustum, `continue` and skip adding it to the `RenderQueue`.
- **Benefit:** Drastically improves CPU performance in large scenes by minimizing the number of `RenderCommand`s processed, sorted, and submitted to OpenGL.
