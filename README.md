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
```

# 3D Line Clipping using Cohen-Sutherland Algorithm


## Description
This OpenGL program demonstrates the Cohen-Sutherland 3D line clipping algorithm against a rectangular box. The visualization includes:
- A 3D clipping box with labeled dimensions
- An original line segment (red)
- The clipped portion of the line (cyan)
- Real-time coordinate display of all points
- Continuous rotation for better 3D understanding

## Key Features
- **3D Cohen-Sutherland Implementation**: Extends the classic 2D algorithm to 3D space
- **Interactive Visualization**:
  - Continuously rotating view
  - Clearly marked original and clipped points
- **Informative Display**:
  - Coordinate labels for all key points
  - Dimension labels for the clipping box
- **Visual Differentiation**:
  - Original line (red)
  - Clipped segment (cyan with yellow endpoints)
  - Clipping box (gray wireframe)

## Technical Implementation
```plaintext
Language: C++
Libraries: OpenGL, GLUT, GLU
Algorithm: Cohen-Sutherland 3D Clipping
```
# 3D Line Clipping using Cyrus-Beck Algorithm

## Description
This OpenGL program demonstrates the Cyrus-Beck 3D line clipping algorithm against a rectangular box. The visualization includes:
- A 3D clipping box with labeled dimensions
- An original line segment (red)
- The clipped portion of the line (cyan)
- Real-time coordinate display of all points
- Continuous rotation for better 3D understanding

## Key Features
- **3D Cyrus-Beck Implementation**: Uses parametric line clipping with plane normals
- **Interactive Visualization**:
  - Continuously rotating view (360° automatic rotation)
  - Clearly marked original and clipped points
- **Informative Display**:
  - Coordinate labels for all key points
  - Dimension labels for the clipping box (5×4×3 units)
- **Visual Differentiation**:
  - Original line (red)
  - Clipped segment (cyan with yellow endpoints)
  - Clipping box (gray wireframe)
  - 3D axes (red=X, green=Y, blue=Z)

## Technical Implementation
```plaintext
Language: C++
Libraries: OpenGL, GLUT, GLU
Algorithm: Cyrus-Beck 3D Clipping (parametric method)
Data Structures: 
  - Plane struct (normal vector + offset)
  - Vector of planes for clipping region
