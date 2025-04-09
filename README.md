# 3D Clipping and Viewing Demonstration

## Description
This OpenGL program demonstrates the concepts of 3D clipping and viewing by visualizing:
- The viewing frustum (near and far clipping planes)
- Multiple 3D objects at different depths
- Interactive camera controls
- Real-time clipping behavior

## Key Concepts Illustrated
1. **View Frustum**: The pyramid-shaped visible region of 3D space
2. **Clipping Planes**:
   - Near plane (objects closer than this are clipped)
   - Far plane (objects farther than this are clipped)
3. **Perspective Projection**: How 3D objects are projected to 2D screen space
4. **Depth Testing**: Handling of object visibility based on depth

## Features
- Multiple colored cubes at different depths
- Toggleable frustum visualization
- Interactive camera movement
- Object rotation controls
- Real-time display of camera parameters

## Controls
| Key | Action |
|-----|--------|
| **Arrow Keys** | Rotate the scene |
| **W** | Move camera forward |
| **S** | Move camera backward |
| **F** | Toggle frustum visibility |
| **ESC** | Exit program |

## Technical Implementation
- **Language**: C++ (also available in Python)
- **Libraries**:
  - OpenGL
  - GLUT (OpenGL Utility Toolkit)
  - GLU (OpenGL Utilities)

## Build Instructions

### Linux
```bash
g++ main.cpp -o main -lGL -lGLU -lglut
