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
### Playstate:
- 