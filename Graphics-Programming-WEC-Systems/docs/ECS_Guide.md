# ECS Guide

## Basic Idea:

- ECS = Entity Component System
- **Entity**: an ID for an object (cube, car, light, etc.)
- **Component**: data attached to that entity (position, model handle, light settings)
- **System**: code that reads or changes that data (moving objects, updating lights, rendering)
- Uses `EnTT` to manage entities and their components
- an entity's components decide what the systems do with it
    - Transform + Render -> can be drawn
    - Transform + PointLight -> gives off light
    - Transform + Render + PointLight -> can be drawn AND gives off light
    - a light doesn't need a visible model to work

## The Registry:

- `entt::registry` is owned by PlayState
- stores the entities and component data
- components of the same type are stored together in their own pools [not inside the Entity wrapper]
- systems get a reference to the registry to access this data
- AssetManager owns the models, meshes, textures, etc. Components only need handles to these assets
- when PlayState is destroyed, its registry and component data are destroyed too

## Entity Class:

- **Holds** (Private variables):
    - `m_EntityHandle`: the entity's ID
    - `m_Registry`: pointer to the registry it belongs to
- **Creating an Entity**:
    - `Entity(&registry)`: creates a new entity in the registry
    - `Entity(handle, &registry)`: wraps an existing entity ID
    - default constructor: empty wrapper [no entity created]
- **Helper Functions**:
    - `AddComponent<T>(...)`: adds a component and returns a reference to it
    - `GetComponent<T>()`: gets a reference to an existing component
    - `HasComponent<T>()`: checks if the entity has that component
    - `EraseComponent<T>()`: removes that component
- [IMP] only add a component once, and only get it if it exists
- copying an Entity copies the ID and registry pointer [both wrappers refer to the same entity]
- destroying the wrapper does not destroy the entity in the registry
- `if (entity)` only checks that the ID isn't `entt::null`. Use `registry.valid(entity)` to check if it is still alive

## Components:

### TransformComponent:

- **Variables**:
    - `position`: position in world space (default = 0)
    - `rotation`: quaternion storing orientation (default = no rotation)
    - `scale`: size along each axis (default = 1)
    - `dirty`: whether the matrices need to be calculated again (default = true)
    - `modelMatrix`: takes model coordinates into world space
    - `normalMatrix`: used to transform normals for lighting
- changing position/rotation/scale doesn't automatically update the matrices
- use TransformSystem's helpers (they set `dirty = true`), or set it manually after changing the data

### RenderComponent:

- holds `modelHandle` (uint32_t): handle to a model in AssetManager
- the model contains nodes with mesh and material handles
- RenderSystem uses these handles to find what it needs to draw
- multiple entities can use the same model handle and have different transforms

### Light Components:

- **PointLightComponent**:
    - color, radius, diffuse/ambient/specular intensities
    - cached world position [updated by LightSystem from the transform]
    - shines in all directions
- **SpotLightComponent**:
    - same basic light settings + inner and outer cutoff angles (in degrees)
    - cached world position and direction [updated by LightSystem]
    - shines in a cone, default local direction is -Z
- **LightOffsetComponent** (optional):
    - `localOffset`: light's position relative to the entity's position
    - `directionOffset`: local direction used by spotlights
    - offsets rotate with the entity [scale is not applied to the position offset]
    - useful when the light should come from a particular part of a model
- **DirectionalLightComponent**:
    - direction and ambient/diffuse/specular colors
    - currently stored in LightSystem, set using `SetDirectionalLight(...)`
    - [IMP] attaching this component to an entity doesn't make the system use it currently
- **GPU Data**:
    - `PointLightUBOData`, `SpotLightUBOData`, and `LightUBO` pack the lighting data for shaders
    - padding fields keep the layout compatible with the shader's std140 layout
    - these are upload structs, not components we add to entities

## Systems:

### TransformSystem:

- **Update**:
    - gets all entities with TransformComponent
    - skips transforms where `dirty == false`
    - calculates `modelMatrix = translate * rotate * scale`
    - calculates normal matrix (inverse transpose of the model matrix's 3x3 part)
    - sets `dirty = false`
- **Movement / Rotation Helpers**:
    - `Translate`: move along world axes
    - `MoveRelative`: move along the object's rotated local axes
    - `Rotate`: rotate around a world axis (angle in degrees)
    - `RotateRelative`: rotate around a local axis (angle in degrees)
- **Setters**:
    - SetPosition, SetRotation, SetScale
    - SetRotation takes either a quaternion or pitchX, yawY, rollZ in degrees
- **Direction Helpers**:
    - GetForward, GetRight, GetUp: local -Z, +X, +Y rotated into world space

### LightSystem:

- **Owns**:
    - light UBO (unique_ptr), bound to base 1
    - directional light settings
    - cached vectors for sorting point lights and spotlights (reused each frame)
- **Update**:
    - finds entities with Transform + PointLight, then Transform + SpotLight
    - calculates their world positions and spotlight directions
    - applies LightOffsetComponent if present
    - sorts to select the lights closest to the camera
    - uploads at most 16 point lights and 8 spotlights [current GPU limits]
    - converts intensities, radius, and cutoff angles into shader data
    - uploads the LightUBO to GPU
- lights beyond these limits still exist in the registry, but aren't uploaded that frame
- use a positive radius, and a non-zero directionOffset for spotlights

### RenderSystem:

- **Render**:
    - gets entities with both RenderComponent and TransformComponent
    - finds each entity's model using AssetManager
    - creates one RenderCommand for each model node (mesh + material + transform matrices)
    - sorts the commands by shader, then material (reduces repeated binds)
    - binds shaders, materials/textures, and meshes when they change
    - sends model and normal matrices, then calls `glDrawElements`
- uses the matrices already calculated by TransformSystem [doesn't calculate transforms itself]
- one entity can produce multiple draw calls if its model has multiple nodes

## Using ECS in PlayState:

### Create an Object:

Example inside `PlayState::Initialize()`, after `floorModel` has been created:

```cpp
Entity floor(&registry);
floor.AddComponent<TransformComponent>(glm::vec3(0.0f, -1.0f, 0.0f));
floor.AddComponent<RenderComponent>(floorModel);

// Keep a wrapper if we want to access this specific object later
entities["floor"] = floor;
```

- AddComponent forwards its arguments to construct the component
- values not supplied use the component's defaults
- the local wrapper can go out of scope; the registry still keeps the entity
- `entities` is just a map for accessing named test objects [not required for ECS to work]

### Change a Transform:

Example inside `PlayState::Update(float dt)`:

```cpp
auto& floorTc = entities.at("floor").GetComponent<TransformComponent>();
transformSystem.Rotate(floorTc, 30.0f * dt, glm::vec3(0.0f, 1.0f, 0.0f));
```

- multiply movement/rotation speed by dt so it depends on time, not frame count
- if changing fields directly:

```cpp
floorTc.position.y = -2.0f;
floorTc.dirty = true;
```

### Create a Light:

```cpp
Entity light(&registry);
light.AddComponent<TransformComponent>(glm::vec3(0.0f, 3.0f, 0.0f));
light.AddComponent<PointLightComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
```

- this creates a red point light with the remaining settings at their defaults
- add RenderComponent too if we want a visible model for it
- change its TransformComponent to move the light [cached light position gets overwritten during Update]

### Remove Components / Entities:

```cpp
if (light.HasComponent<PointLightComponent>()) {
    light.EraseComponent<PointLightComponent>();
}

registry.destroy(light);
```

- removing the light component stops it giving off light, but keeps the entity
- `registry.destroy(...)` removes the entity and all its components
- remove any wrappers for that entity from the map too [they now refer to a destroyed entity]
- don't use old component references after their component/entity is removed

## Views (How Systems Find Entities):

```cpp
auto view = registry.view<TransformComponent, RenderComponent>();

view.each([](auto entity, auto& tc, auto& rc) {
    // Runs once for each entity that has BOTH components
});
```

- view filters entities by the component types requested
- no need to store every object in a map or check every entity manually
- to add new behaviour: create a component for its data, then a system that processes a matching view
- call that system from PlayState in the order it needs to run

## Current Frame Order:

- **PlayState::Update**:
    - checks pause input [returns immediately if pausing]
    - changes transforms for the test animations
    - TransformSystem.Update -> refreshes dirty matrices
    - LightSystem.Update -> updates lights using the camera's current position
    - Camera.Update -> handles camera movement
    - Camera.UpdateUBO -> uploads view/projection matrices
- **PlayState::Render**:
    - RenderSystem.Render -> builds commands and draws the scene
- [IMP] transform changes should happen before TransformSystem.Update so rendering uses the new matrices
- currently light selection runs before camera movement [uses the camera position from before that frame's movement]
- Camera and Input are still separate from ECS [can be changed later]
