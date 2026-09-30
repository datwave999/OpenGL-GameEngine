# Architecture Overview of the Engine

## Client runs main.cpp
- owns application app (stack allocation)
- initializes and runs app
- dumps any memory leaks after termination of main() using <crtdbg.h>

## Entry Point to engine (Application class):
- __Owns__ (Private variables):
	- The window (unique_ptr): because window must be owned only by application and should not be copied
	- AssetManager assets (stack alloc): global asset manager [Add more info here later]
	- State Stack vector (holds unique state ptrs): to manage game states
	- global variables to track dt and fps
- __Life Cycle Functions__:
	- Initialization: 
		- creates and stores unique_ptr of window
		- initializes assets
		- global engine settings (openGL and stbi)
		- calls glfw input functions which calls the callback functions of the Static Input class (asynchronous callbacks)
		- starts the game by pushing the first state (playstate)'s unique ptr (gives it a pointer to thyself)
	- Run:
		- The main engine's loop runs while window is alive (pressing x stops the while loop), also terminates if state stack gets empty.
		- calculates dt and fps, displays fps
		- calls update and render function
		- calls input's endFrame function [mentioned in Handling Inputs]
		- Swaps Buffers (display the current back buffer)
	- State Management: Helpers for pushing and popping states
		- PushState: moves the unique_ptr of the state passed into it to the state stack
		- PopState: pops state
	- Update: Updates the entire application (engine)
		- only calls update function of the top state on the State Stack
	- Render:
		- clears the front buffer
		- Render states (highest opaque to top)
	- Shutdown:
		- clears all states

## Handling Inputs:
- Uses Asynchronous callbacks from GLFW to handle inputs
- Static class: so that it can be accessed globally (and only one Input exists) [Bad for ECS architecture, change later]
- whenever an input is detected, GLFW runs a callback function provided by this class
- the callback function updates its internal variables and then calculates and updates the State Arrays for the keys and mouse buttons
- Getters used to get key and mouse states
- these getters can then be polled in [other files] to program what different inputs do [Change by using Event queue instead of polling]
- EndFrame function: resets KeysChanged array to False (since frame ended), resets delta values (dx, dy) to 0

## The Main Window: OpenGL State
- **Responsibilities**:
	- Provides canvas to draw on (gets a window from the OS)
		- Owns the GLFWwindow* ptr
		- framebuffer size callback (handle resizing the window)
	- Initialize the OpenGL Context (Because openGL is a state machine)
		- when window is created, glfw creates a default openGL context (GLFWwindow owns the context)
- **Working**:
	- Constructor (Called from app.Initialize() and stores in a unique_ptr):
		- Initializes GLFW (the window, inputs and context library)
		- sets OpenGL version 4.6 core
		- creates the window
		- Tie Window class's window instance to GLFWwindow (For resizing and changing internal pointers later in static function)
		- [IMP] Makes Context Current (GLFWwindow owns the context which is not set as the current context, any openGL function calls will assume this context/window now)\
		- load openGL (gl....) functions using GLAD (called after making context current so that glad knows which GPU you are using and fetches functions according to it)
		- make viewport = window's size (viewport and window size are different things)
		- callback for resizing window (window resize -> glfw functions calls the callback function -> callback function updates viewport and also internal width and height variables)
	- Helper Functions (Abstaction): 
		- window should close: checks the close flag
		- buffer swap
		- poll events: polls the internal events queue (that stores the events like key presses)
		- getting width and height, setting window title
	- Destructor:
		- destroys the window (and the context)
		- terminates glfw
		- called automatically when app is destroyed (app owns window unique_ptr)

## Camera
Currently a standalone C++ Class [Will be changed to be a component in the ECS architecture]
- **Variables**:
	- Position, Front, Up, Right, WorldUp
	- Angles
	- Camera Settings like movement speed, sens, fov
	- The camera UBO (Buffer object) which is used to update the camera matrices in shaders
- **Working**:
	- Constructor (takes in position but has a default val):
		- Creates camera, init all variables to default values
		- calls helper function to update camera vectors (front, right and up)
		- init camera UBO which creates a buffer [Camera owns its UBO, changes with ECS architecture]
	- Update:
		- Polling the Input class's static arrays for inputs and managing movement, looking around, changing fov, etc.
		- automatically calls camera update vectors if pitch/yaw is changed
	- UpdateUBO:
		- Uses the helper functions to calculate projection and view matrices, packs them into CameraMatrices
		- Calls the Buffer's updateData function, which updates the UBO in GPU memory (Binded to base 0)
	- Getters:
		- for position, front, fov
		- calculations for view and projection matrices
	- Setters:
		- for Movement speed (not currently used anywhere)

## States (PlayState and PauseState):
### PlayState:
- **Owns** (Private variables):
	- Camera (unique_ptr)
	- ECS Core: `entt::registry` (contiguous memory database where all components live)
	- Systems: `TransformSystem`, `LightSystem`, and `RenderSystem`
	- Entities Map: `std::unordered_map<std::string, Entity>` used as a sandbox to track specific objects [Anti-pattern: Just for tesing purposes, but doing this defeats the purpose of ECS]
- **Working / Life Cycle**:
	- Initialize: 
		- Sets input mode (locks and hides cursor for FPS controls)
		- Uses the global AssetManager to compile shaders, generate standard meshes, load textures, and load 3D models via Assimp
		- Creates Entities, adds components (Transform, Render, Lights) to them via ECS, and stores them in the entities map
	- Update:
		- Checks for ESC key -> calls `app->PushState` to overlay the PauseState and immediately `return;` (freezing PlayState logic)
		- accesses specific entities from the map and manipulates their transforms (rotating and movement stuff) [testing purposes]
		- System Pipeline: calls `transformSystem.Update` (calculates local-to-world matrices) and `lightSystem.Update` (sorts lights and pushes UBO to GPU)
		- Updates Camera (movement, fov) and pushes matrices to GPU UBO
	- Render:
		- No raw openGL math. Simply delegates to `renderSystem.Render`, passing the registry, assets, and camera position
	- isOpaque: 
		- Returns `true`. The Application loop stops iterating downwards when it sees this

### PauseState:
- **Owns** (Private variables):
	- `overlayShaderHandle` and `overlayMeshHandle` (uint32_t): Extremely lightweight, holds no complex objects.
- **Working / Life Cycle**:
	- Initialize: 
		- Frees the mouse cursor (`GLFW_CURSOR_NORMAL`) for menu interaction
		- Fetches the overlay shader and 2D overlay mesh handles from AssetManager (utilizes DOD caching; doesn't re-compile existing assets)
	- Update:
		- Checks for ESC key -> recaptures the mouse and calls `app->PopState()` (pops itself off the stack, resuming PlayState on the next frame)
	- Render:
		- Manually overrides openGL state (bypass ECS): Enables `GL_BLEND` (for transparency) and disables `GL_DEPTH_TEST` (so it draws over the 3D world)
		- Binds the overlay shader and mesh, executes raw `glDrawElements` draw call
		- Restores openGL state (enables depth test, disables blend) to not break the next frame
	- isOpaque: 
		- Returns `false`. Tells the Application to render the state below it (PlayState) first, allowing this state to draw a transparent overlay on top.