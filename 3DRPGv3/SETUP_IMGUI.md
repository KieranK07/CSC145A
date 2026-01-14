# ImGui Setup Instructions

To use the text rendering features in the pause menu and debug info, you need to add ImGui to your project:

## Steps:

1. Download ImGui from: https://github.com/ocornut/imgui/releases
   - Download the latest release (ZIP file)

2. Extract the ImGui folder to `libs/imgui/`
   - Your structure should look like:
   ```
   libs/
     imgui/
       imgui.h
       imgui.cpp
       imgui_demo.cpp
       imgui_draw.cpp
       imgui_tables.cpp
       imgui_widgets.cpp
       backends/
         imgui_impl_glfw.h
         imgui_impl_glfw.cpp
         imgui_impl_opengl3.h
         imgui_impl_opengl3.cpp
       ... (other files)
   ```

3. Rebuild your project:
   - Delete the `build/` folder
   - Run CMake again to regenerate the build files
   - Recompile the project

## Features Now Available:

- **Pause Menu**: Press ESC to pause. You'll see a nice ImGui window with a "Resume Game" button
- **Debug Info**: Top-left corner shows FPS, player position, and grounded status
- **Text Rendering**: All text is properly rendered (no more invisible text!)

## Benefits:

- ImGui handles all UI rendering efficiently
- Text looks clean and professional
- Easy to add more UI elements later
- Mouse input is handled automatically

That's it! ImGui will be compiled as part of your project.
