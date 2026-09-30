# Dev Log : 9/30/2026
## First Dev Log, relearning the entire codebase
- ### Writing detailed documentation for:
	- **CORE**: Application, Window, Input, Camera, States
- ### Bug Fixes:
	- Input::keyCallBack: returned if unmapped key is called preventing crashes
	- Resize callback function in window not updating internal width and height variables causing stretching or squashing of models when resizing
	- removed ProcessScrollWheel function in the camera, Update camera function does that fov changing in itself now.
	- stopped storing shader id in playstate's private variable as shader id is attached to object now.