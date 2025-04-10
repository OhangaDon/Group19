# Import required OpenGL and system libraries
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from OpenGL.GLUT import GLUT_BITMAP_HELVETICA_12
import sys
import math

# Global state variables
angle_x, angle_y = 0, 0        # Rotation angles for user-controlled rotation
camera_z = 15.0                # Distance of the camera along the Z axis
show_frustum = True           # Toggle for showing the frustum view
near_plane, far_plane = 2.0, 50.0  # Clipping planes

# List of 3D objects with positions, sizes, and colors
objects = [
    {'x': 0, 'y': 0, 'z': 0, 'size': 1.0, 'color': (1.0, 0.0, 0.0)},   # Center red cube
    {'x': 3, 'y': 2, 'z': -10, 'size': 0.8, 'color': (0.0, 1.0, 0.0)}, # Far green cube
    {'x': -2, 'y': -1, 'z': -5, 'size': 0.6, 'color': (0.0, 0.0, 1.0)},# Mid blue cube
    {'x': 1, 'y': 1.5, 'z': -2, 'size': 0.4, 'color': (1.0, 1.0, 0.0)},# Near yellow cube
    {'x': 0, 'y': 0, 'z': -far_plane*0.9, 'size': 0.5, 'color': (1.0, 0.0, 1.0)}  # Almost clipped purple cube
]

# Initialize OpenGL settings
def init():
    glClearColor(0.1, 0.1, 0.1, 1.0)  # Background color (dark gray)
    glEnable(GL_DEPTH_TEST)          # Enable depth testing for correct z-order rendering
    glEnable(GL_LIGHTING)            # Enable lighting
    glEnable(GL_LIGHT0)              # Enable light source 0

    # Set light properties (position, diffuse and ambient color)
    glLightfv(GL_LIGHT0, GL_POSITION, [1.0, 1.0, 1.0, 0.0])
    glLightfv(GL_LIGHT0, GL_DIFFUSE, [1.0, 1.0, 1.0, 1.0])
    glLightfv(GL_LIGHT0, GL_AMBIENT, [0.2, 0.2, 0.2, 1.0])

    # Enable color tracking for materials
    glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE)

# Draw a cube at the specified position, size, and color
def draw_cube(x, y, z, size, color):
    glPushMatrix()
    glTranslatef(x, y, z)
    glScalef(size, size, size)
    
    glBegin(GL_QUADS)
    glColor3f(*color)

    # Define each face of the cube and assign normals for lighting
    # Front face
    glNormal3f(0.0, 0.0, 1.0)
    glVertex3f(-1.0, -1.0,  1.0)
    glVertex3f( 1.0, -1.0,  1.0)
    glVertex3f( 1.0,  1.0,  1.0)
    glVertex3f(-1.0,  1.0,  1.0)

    # Back face
    glNormal3f(0.0, 0.0, -1.0)
    glVertex3f(-1.0, -1.0, -1.0)
    glVertex3f(-1.0,  1.0, -1.0)
    glVertex3f( 1.0,  1.0, -1.0)
    glVertex3f( 1.0, -1.0, -1.0)

    # Top face
    glNormal3f(0.0, 1.0, 0.0)
    glVertex3f(-1.0,  1.0, -1.0)
    glVertex3f(-1.0,  1.0,  1.0)
    glVertex3f( 1.0,  1.0,  1.0)
    glVertex3f( 1.0,  1.0, -1.0)

    # Bottom face
    glNormal3f(0.0, -1.0, 0.0)
    glVertex3f(-1.0, -1.0, -1.0)
    glVertex3f( 1.0, -1.0, -1.0)
    glVertex3f( 1.0, -1.0,  1.0)
    glVertex3f(-1.0, -1.0,  1.0)

    # Right face
    glNormal3f(1.0, 0.0, 0.0)
    glVertex3f( 1.0, -1.0, -1.0)
    glVertex3f( 1.0,  1.0, -1.0)
    glVertex3f( 1.0,  1.0,  1.0)
    glVertex3f( 1.0, -1.0,  1.0)

    # Left face
    glNormal3f(-1.0, 0.0, 0.0)
    glVertex3f(-1.0, -1.0, -1.0)
    glVertex3f(-1.0, -1.0,  1.0)
    glVertex3f(-1.0,  1.0,  1.0)
    glVertex3f(-1.0,  1.0, -1.0)

    glEnd()
    glPopMatrix()

# Draw the viewing frustum boundaries (optional)
def draw_frustum():
    if not show_frustum:
        return

    glDisable(GL_LIGHTING)
    
    # Draw near clipping plane (green square)
    glBegin(GL_LINE_LOOP)
    glColor3f(0.0, 1.0, 0.0)
    glVertex3f(-1.0, -1.0, -near_plane)
    glVertex3f( 1.0, -1.0, -near_plane)
    glVertex3f( 1.0,  1.0, -near_plane)
    glVertex3f(-1.0,  1.0, -near_plane)
    glEnd()
    
    # Draw far clipping plane (red square)
    glBegin(GL_LINE_LOOP)
    glColor3f(1.0, 0.0, 0.0)
    glVertex3f(-5.0, -5.0, -far_plane)
    glVertex3f( 5.0, -5.0, -far_plane)
    glVertex3f( 5.0,  5.0, -far_plane)
    glVertex3f(-5.0,  5.0, -far_plane)
    glEnd()
    
    # Connect corners of near and far planes
    glBegin(GL_LINES)
    glColor3f(0.7, 0.7, 0.7)
    glVertex3f(-1.0, -1.0, -near_plane); glVertex3f(-5.0, -5.0, -far_plane)
    glVertex3f( 1.0, -1.0, -near_plane); glVertex3f( 5.0, -5.0, -far_plane)
    glVertex3f( 1.0,  1.0, -near_plane); glVertex3f( 5.0,  5.0, -far_plane)
    glVertex3f(-1.0,  1.0, -near_plane); glVertex3f(-5.0,  5.0, -far_plane)
    glEnd()
    
    glEnable(GL_LIGHTING)

# Render 2D overlay text on screen
def draw_text(x, y, text):
    glDisable(GL_LIGHTING)
    glMatrixMode(GL_PROJECTION)
    glPushMatrix()
    glLoadIdentity()
    gluOrtho2D(0, 600, 0, 600)
    glMatrixMode(GL_MODELVIEW)
    glPushMatrix()
    glLoadIdentity()
    glColor3f(1, 1, 1)
    glRasterPos2i(x, y)
    for ch in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord(ch))
    glPopMatrix()
    glMatrixMode(GL_PROJECTION)
    glPopMatrix()
    glMatrixMode(GL_MODELVIEW)
    glEnable(GL_LIGHTING)

# Main display callback for GLUT
def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    # Set up projection and camera
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(60, 1.0, near_plane, far_plane)

    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()
    gluLookAt(0, 0, camera_z, 0, 0, 0, 0, 1, 0)

    # Apply rotation
    glRotatef(angle_x, 1.0, 0.0, 0.0)
    glRotatef(angle_y, 0.0, 1.0, 0.0)

    # Draw each cube
    for obj in objects:
        draw_cube(obj['x'], obj['y'], obj['z'], obj['size'], obj['color'])

    # Draw frustum lines if enabled
    draw_frustum()

    # Draw overlay text
    draw_text(10, 580, f"Camera Z: {camera_z:.1f}  Near: {near_plane}  Far: {far_plane}")
    draw_text(10, 560, "Arrow keys: Rotate  W/S: Move camera  F: Toggle frustum")

    glutSwapBuffers()

# Handle standard keyboard input (W, S, F, ESC)
def keyboard(key, x, y):
    global camera_z, show_frustum

    key = key.decode('utf-8').lower()
    if key == '\x1b':  # ESC key
        glutLeaveMainLoop()
    elif key == 'w':
        camera_z -= 0.5  # Move camera closer
    elif key == 's':
        camera_z += 0.5  # Move camera further
    elif key == 'f':
        show_frustum = not show_frustum  # Toggle frustum drawing
    glutPostRedisplay()

# Handle special keys (arrow keys for rotation)
def special_keys(key, x, y):
    global angle_x, angle_y
    if key == GLUT_KEY_UP:
        angle_x += 5
    elif key == GLUT_KEY_DOWN:
        angle_x -= 5
    elif key == GLUT_KEY_LEFT:
        angle_y += 5
    elif key == GLUT_KEY_RIGHT:
        angle_y -= 5
    glutPostRedisplay()

# Main function that sets up the GLUT environment
def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH)
    glutInitWindowSize(600, 600)
    glutCreateWindow(b"Enhanced 3D Clipping & Viewing Demonstration")
    
    init()
    glutDisplayFunc(display)
    glutKeyboardFunc(keyboard)
    glutSpecialFunc(special_keys)
    glutMainLoop()

# Entry point
if __name__ == "__main__":
    main()
