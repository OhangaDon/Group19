import sys
from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from OpenGL.GLUT import GLUT_BITMAP_HELVETICA_12
import math

# Define the 3D box boundaries
xmin, xmax = 0.0, 5.0
ymin, ymax = 0.0, 4.0
zmin, zmax = 0.0, 3.0

angle = 0.0  # Rotation angle for animation

# Outcode bitmasks for Cohen-Sutherland algorithm
INSIDE = 0     # 000000
LEFT = 1       # 000001
RIGHT = 2      # 000010
BOTTOM = 4     # 000100
TOP = 8        # 001000
NEAR = 16      # 010000
FAR_ = 32      # 100000

# Initial line endpoints (some points lie outside the box)
x0, y0, z0 = -1.0, 2.0, 1.0
x1, y1, z1 = 6.0, 5.0, 4.0

# Function to render text at a 3D position
def draw_text(x, y, z, text):
    glRasterPos3f(x, y, z)
    for character in text:
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, ord(character))

# Format coordinates nicely
def coord_to_str(x, y, z):
    return f"({x:.1f}, {y:.1f}, {z:.1f})"

# Assign an outcode for a point based on its position relative to the clipping box
def compute_outcode(x, y, z):
    code = INSIDE
    if x < xmin: code |= LEFT
    elif x > xmax: code |= RIGHT
    if y < ymin: code |= BOTTOM
    elif y > ymax: code |= TOP
    if z < zmin: code |= NEAR
    elif z > zmax: code |= FAR_
    return code

# Cohen-Sutherland 3D line clipping algorithm
def cohen_sutherland_clip(x0, y0, z0, x1, y1, z1):
    outcode0 = compute_outcode(x0, y0, z0)
    outcode1 = compute_outcode(x1, y1, z1)
    accept = False

    while True:
        if not (outcode0 | outcode1):
            # Bitwise OR is 0: both points inside, trivially accept
            accept = True
            break
        elif outcode0 & outcode1:
            # Bitwise AND is not 0: line is trivially outside, reject
            break
        else:
            # Line needs clipping: choose an outside point
            outcode_out = outcode0 if outcode0 else outcode1

            # Calculate intersection with corresponding boundary
            if outcode_out & TOP:
                y = ymax
                t = (ymax - y0) / (y1 - y0)
                x = x0 + t * (x1 - x0)
                z = z0 + t * (z1 - z0)
            elif outcode_out & BOTTOM:
                y = ymin
                t = (ymin - y0) / (y1 - y0)
                x = x0 + t * (x1 - x0)
                z = z0 + t * (z1 - z0)
            elif outcode_out & RIGHT:
                x = xmax
                t = (xmax - x0) / (x1 - x0)
                y = y0 + t * (y1 - y0)
                z = z0 + t * (z1 - z0)
            elif outcode_out & LEFT:
                x = xmin
                t = (xmin - x0) / (x1 - x0)
                y = y0 + t * (y1 - y0)
                z = z0 + t * (z1 - z0)
            elif outcode_out & FAR_:
                z = zmax
                t = (zmax - z0) / (z1 - z0)
                x = x0 + t * (x1 - x0)
                y = y0 + t * (y1 - y0)
            else:  # NEAR
                z = zmin
                t = (zmin - z0) / (z1 - z0)
                x = x0 + t * (x1 - x0)
                y = y0 + t * (y1 - y0)

            # Replace the point outside with intersection point and update outcode
            if outcode_out == outcode0:
                x0, y0, z0 = x, y, z
                outcode0 = compute_outcode(x0, y0, z0)
            else:
                x1, y1, z1 = x, y, z
                outcode1 = compute_outcode(x1, y1, z1)

    return accept, x0, y0, z0, x1, y1, z1

# Draw the clipping box (wireframe cube)
def draw_box():
    x = [xmin, xmax]
    y = [ymin, ymax]
    z = [zmin, zmax]

    glColor3f(0.8, 0.8, 0.8)
    glLineWidth(2.0)
    glBegin(GL_LINES)
    for i in range(2):
        for j in range(2):
            glVertex3f(x[0], y[i], z[j]); glVertex3f(x[1], y[i], z[j])
            glVertex3f(x[i], y[0], z[j]); glVertex3f(x[i], y[1], z[j])
            glVertex3f(x[i], y[j], z[0]); glVertex3f(x[i], y[j], z[1])
    glEnd()

    # Dimension labels
    glColor3f(1, 1, 1)
    draw_text(xmax + 0.1, ymin + 0.1, zmin, "Length: 5 cm")
    draw_text(xmin + 0.1, ymax + 0.1, zmin, "Height: 4 cm")
    draw_text(xmin + 0.1, ymin + 0.1, zmax + 0.1, "Width: 3 cm")

# Main display function
def display():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()

    # Camera setup
    gluLookAt(10, 8, 10, 2.5, 2, 1.5, 0, 1, 0)

    # Apply rotation to the entire scene
    glRotatef(angle, 0, 1, 0)

    # Draw axis lines
    glLineWidth(1.5)
    glBegin(GL_LINES)
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(6, 0, 0)  # X - red
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0)  # Y - green
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 4)  # Z - blue
    glEnd()

    draw_box()  # Draw the clipping cube

    # Draw original line in red
    glColor3f(1, 0, 0)
    glLineWidth(2.0)
    glBegin(GL_LINES)
    glVertex3f(x0, y0, z0)
    glVertex3f(x1, y1, z1)
    glEnd()

    # Label endpoints
    glColor3f(1, 1, 1)
    draw_text(x0 + 0.1, y0 + 0.1, z0, "P1 " + coord_to_str(x0, y0, z0))
    draw_text(x1 + 0.1, y1 + 0.1, z1, "P2 " + coord_to_str(x1, y1, z1))

    # Clip the line using Cohen-Sutherland algorithm
    accept, cx0, cy0, cz0, cx1, cy1, cz1 = cohen_sutherland_clip(x0, y0, z0, x1, y1, z1)
    if accept:
        # Draw clipped line segment in cyan
        glColor3f(0.0, 1.0, 1.0)
        glLineWidth(4.0)
        glBegin(GL_LINES)
        glVertex3f(cx0, cy0, cz0)
        glVertex3f(cx1, cy1, cz1)
        glEnd()

        # Highlight clipped endpoints in yellow
        glPointSize(8)
        glColor3f(1.0, 1.0, 0.0)
        glBegin(GL_POINTS)
        glVertex3f(cx0, cy0, cz0)
        glVertex3f(cx1, cy1, cz1)
        glEnd()

        # Label clipped endpoints
        glColor3f(0.9, 1.0, 0.9)
        draw_text(cx0 + 0.1, cy0 + 0.1, cz0, "C1 " + coord_to_str(cx0, cy0, cz0))
        draw_text(cx1 + 0.1, cy1 + 0.1, cz1, "C2 " + coord_to_str(cx1, cy1, cz1))

    glutSwapBuffers()

# Timer function to update rotation
def timer(value):
    global angle
    angle += 0.5
    if angle > 360:
        angle -= 360
    glutPostRedisplay()
    glutTimerFunc(16, timer, 0)  # ~60 FPS

# OpenGL initialization
def init():
    glEnable(GL_DEPTH_TEST)
    glMatrixMode(GL_PROJECTION)
    gluPerspective(60, 1.0, 1.0, 50.0)
    glMatrixMode(GL_MODELVIEW)

# Main function to start GLUT loop
def main():
    glutInit(sys.argv)
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(900, 800)
    glutCreateWindow(b"Cohen-Sutherland 3D Clipping with Rotation and Coordinates")
    init()
    glutDisplayFunc(display)
    glutTimerFunc(0, timer, 0)
    glutMainLoop()

if __name__ == "__main__":
    main()
