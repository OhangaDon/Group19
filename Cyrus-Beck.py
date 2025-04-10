import sys
import math
import OpenGL.GL as gl
import OpenGL.GLUT as glut
import OpenGL.GLU as glu
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from OpenGL.GLUT import GLUT_BITMAP_HELVETICA_12

# Define the dimensions of the clipping box
xmin, xmax = 0.0, 5.0
ymin, ymax = 0.0, 4.0
zmin, zmax = 0.0, 3.0

angle = 0.0  # Angle for rotation animation

# Define six clipping planes using their normals and distances (Ax + By + Cz = d)
planes = [
    {'normal': [1, 0, 0], 'd': xmin},     # Left face of the box
    {'normal': [-1, 0, 0], 'd': -xmax},   # Right face
    {'normal': [0, 1, 0], 'd': ymin},     # Bottom face
    {'normal': [0, -1, 0], 'd': -ymax},   # Top face
    {'normal': [0, 0, 1], 'd': zmin},     # Near face
    {'normal': [0, 0, -1], 'd': -zmax}    # Far face
]

# Helper function to render 3D text in OpenGL
def draw_text(x, y, z, text, font=GLUT_BITMAP_HELVETICA_12):
    gl.glRasterPos3f(x, y, z)
    for c in text:
        glut.glutBitmapCharacter(font, ord(c))

# Convert coordinates to string format for labels
def coord_to_str(x, y, z):
    return f"({x:.1f}, {y:.1f}, {z:.1f})"

# Compute point along a parametric line
def parametric_line(t, x0, y0, z0, x1, y1, z1):
    x = x0 + t * (x1 - x0)
    y = y0 + t * (y1 - y0)
    z = z0 + t * (z1 - z0)
    return x, y, z

# Compute the dot product of two vectors
def dot_product(a, b):
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]

# Cyrus-Beck 3D line clipping algorithm
def cyrus_beck_clip(x0, y0, z0, x1, y1, z1):
    t0, t1 = 0.0, 1.0  # Initialize entry and exit points
    accept = True

    for plane in planes:
        p0 = [x0, y0, z0]  # Start point
        p1 = [x1, y1, z1]  # End point
        dir = [p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]]  # Direction vector
        denom = dot_product(plane['normal'], dir)

        if denom > 0:  # Line is entering the half-space
            num = dot_product(plane['normal'], p0) - plane['d']
            t = -num / denom
            if t > t0: t0 = t  # Update t0 to furthest entrance
        elif denom < 0:  # Line is leaving the half-space
            num = dot_product(plane['normal'], p0) - plane['d']
            t = -num / denom
            if t < t1: t1 = t  # Update t1 to closest exit
        elif dot_product(plane['normal'], p0) - plane['d'] > 0:
            # Line is outside and parallel to the plane
            accept = False
            break

    # If there's an overlap between entry and exit points, accept the clipped line
    if accept and t0 < t1:
        x0, y0, z0 = parametric_line(t0, x0, y0, z0, x1, y1, z1)
        x1, y1, z1 = parametric_line(t1, x0, y0, z0, x1, y1, z1)
        return x0, y0, z0, x1, y1, z1
    return None

# Define original 3D line endpoints
x0, y0, z0 = 1, 1, 1
x1, y1, z1 = 4, 3, 2

# Draw the clipping box (wireframe)
def draw_box():
    x = [xmin, xmax]
    y = [ymin, ymax]
    z = [zmin, zmax]

    gl.glColor3f(0.8, 0.8, 0.8)  # Light gray color
    gl.glLineWidth(2.0)
    gl.glBegin(gl.GL_LINES)
    for i in range(2):
        for j in range(2):
            gl.glVertex3f(x[0], y[i], z[j])
            gl.glVertex3f(x[1], y[i], z[j])
            gl.glVertex3f(x[i], y[0], z[j])
            gl.glVertex3f(x[i], y[1], z[j])
            gl.glVertex3f(x[i], y[j], z[0])
            gl.glVertex3f(x[i], y[j], z[1])
    gl.glEnd()

    # Add text labels for box dimensions
    gl.glColor3f(1, 1, 1)
    draw_text(xmax + 0.1, ymin + 0.1, zmin, "Length: 5 cm")   # X-axis
    draw_text(xmin + 0.1, ymax + 0.1, zmin, "Height: 4 cm")   # Y-axis
    draw_text(xmin + 0.1, ymin + 0.1, zmax + 0.1, "Width: 3 cm")  # Z-axis

# Main display callback
def display():
    global angle
    gl.glClear(gl.GL_COLOR_BUFFER_BIT | gl.GL_DEPTH_BUFFER_BIT)
    gl.glLoadIdentity()

    # Set the camera using gluLookAt (eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ)
    glu.gluLookAt(10, 8, 10, 2.5, 2, 1.5, 0, 1, 0)
    gl.glRotatef(angle, 0, 1, 0)  # Rotate scene around Y-axis

    # Draw X, Y, Z axes
    gl.glLineWidth(1.5)
    gl.glBegin(gl.GL_LINES)
    gl.glColor3f(1, 0, 0); gl.glVertex3f(0, 0, 0); gl.glVertex3f(6, 0, 0)  # X-axis
    gl.glColor3f(0, 1, 0); gl.glVertex3f(0, 0, 0); gl.glVertex3f(0, 5, 0)  # Y-axis
    gl.glColor3f(0, 0, 1); gl.glVertex3f(0, 0, 0); gl.glVertex3f(0, 0, 4)  # Z-axis
    gl.glEnd()

    draw_box()  # Draw the 3D box

    # Draw the original line (before clipping)
    gl.glColor3f(1, 0, 0)  # Red
    gl.glLineWidth(2.0)
    gl.glBegin(gl.GL_LINES)
    gl.glVertex3f(x0, y0, z0)
    gl.glVertex3f(x1, y1, z1)
    gl.glEnd()

    # Add labels for endpoints
    gl.glColor3f(1, 1, 1)
    draw_text(x0 + 0.1, y0 + 0.1, z0, f"P1 {coord_to_str(x0, y0, z0)}")
    draw_text(x1 + 0.1, y1 + 0.1, z1, f"P2 {coord_to_str(x1, y1, z1)}")

    # Perform Cyrus-Beck clipping and draw the clipped segment
    clipped = cyrus_beck_clip(x0, y0, z0, x1, y1, z1)
    if clipped:
        cx0, cy0, cz0, cx1, cy1, cz1 = clipped

        # Draw clipped line segment
        gl.glColor3f(0.0, 1.0, 1.0)  # Cyan
        gl.glLineWidth(4.0)
        gl.glBegin(gl.GL_LINES)
        gl.glVertex3f(cx0, cy0, cz0)
        gl.glVertex3f(cx1, cy1, cz1)
        gl.glEnd()

        # Highlight clipped points
        gl.glPointSize(8)
        gl.glColor3f(1.0, 1.0, 0.0)  # Yellow
        gl.glBegin(gl.GL_POINTS)
        gl.glVertex3f(cx0, cy0, cz0)
        gl.glVertex3f(cx1, cy1, cz1)
        gl.glEnd()

        # Label clipped endpoints
        gl.glColor3f(0.9, 1.0, 0.9)
        draw_text(cx0 + 0.1, cy0 + 0.1, cz0, f"C1 {coord_to_str(cx0, cy0, cz0)}")
        draw_text(cx1 + 0.1, cy1 + 0.1, cz1, f"C2 {coord_to_str(cx1, cy1, cz1)}")

    glut.glutSwapBuffers()  # Swap front and back buffers for smooth animation

# Timer callback for rotating animation
def timer(value):
    global angle
    angle += 0.5  # Increase angle slightly
    if angle > 360:
        angle -= 360
    glut.glutPostRedisplay()
    glut.glutTimerFunc(16, timer, 0)  # ~60 FPS

# OpenGL initialization
def init():
    gl.glEnable(gl.GL_DEPTH_TEST)  # Enable depth buffering
    gl.glMatrixMode(gl.GL_PROJECTION)
    glu.gluPerspective(60, 1.0, 1.0, 50.0)  # Perspective view
    gl.glMatrixMode(gl.GL_MODELVIEW)

# Program entry point
def main():
    glut.glutInit(sys.argv)
    glut.glutInitDisplayMode(glut.GLUT_RGB | glut.GLUT_DOUBLE | glut.GLUT_DEPTH)
    glut.glutInitWindowSize(900, 800)
    glut.glutCreateWindow("Cyrus-Beck 3D Clipping with Rotation and Coordinates")
    init()
    glut.glutDisplayFunc(display)
    glut.glutTimerFunc(0, timer, 0)
    glut.glutMainLoop()

if __name__ == "__main__":
    main()
