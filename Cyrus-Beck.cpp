#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

// Define box (clipping volume) dimensions
const float xmin = 0.0f, xmax = 5.0f;
const float ymin = 0.0f, ymax = 4.0f;
const float zmin = 0.0f, zmax = 3.0f;

float angle = 0.0f;  // angle for continuous rotation animation

// Define a structure to represent a plane (normal vector and offset)
struct Plane {
    float normal[3];  // Normal vector to the plane
    float d;          // Offset (distance from origin in plane equation)
};

// Clipping planes for the 3D box (each plane clips part of the volume)
std::vector<Plane> planes = {
    {{1, 0, 0}, xmin},     // Left plane
    {{-1, 0, 0}, -xmax},   // Right plane
    {{0, 1, 0}, ymin},     // Bottom plane
    {{0, -1, 0}, -ymax},   // Top plane
    {{0, 0, 1}, zmin},     // Near plane
    {{0, 0, -1}, -zmax}    // Far plane
};

// Function to draw text labels at a 3D position
void drawText(float x, float y, float z, std::string text, void *font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos3f(x, y, z);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

// Convert 3D coordinate to string format (for labeling)
std::string coordToStr(float x, float y, float z) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);
    ss << "(" << x << ", " << y << ", " << z << ")";
    return ss.str();
}

// Compute a point on a line segment using parametric form
void parametricLine(float t, float x0, float y0, float z0, float x1, float y1, float z1, float &x, float &y, float &z) {
    x = x0 + t * (x1 - x0);
    y = y0 + t * (y1 - y0);
    z = z0 + t * (z1 - z0);
}

// Compute the dot product of two vectors
float dotProduct(const float a[3], const float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Cyrus-Beck line clipping algorithm for 3D
bool cyrusBeckClip(float &x0, float &y0, float &z0, float &x1, float &y1, float &z1) {
    float t0 = 0.0f, t1 = 1.0f;  // Range of valid t values for visible portion
    bool accept = true;

    for (const auto &plane : planes) {
        float p0[3] = {x0, y0, z0};
        float p1[3] = {x1, y1, z1};
        float dir[3] = {p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]};  // Direction vector of the line
        float denom = dotProduct(plane.normal, dir);

        float num = dotProduct(plane.normal, p0) - plane.d;

        if (denom > 0) {  // Line is entering the region
            float t = -num / denom;
            if (t > t0) t0 = t;
        } else if (denom < 0) {  // Line is exiting the region
            float t = -num / denom;
            if (t < t1) t1 = t;
        } else if (num > 0) {  // Line is parallel and outside the plane
            accept = false;
            break;
        }
    }

    // If line lies partially or completely inside the volume
    if (accept && t0 < t1) {
        parametricLine(t0, x0, y0, z0, x1, y1, z1, x0, y0, z0);
        parametricLine(t1, x0, y0, z0, x1, y1, z1, x1, y1, z1);
    }

    return accept;
}

// Original 3D line endpoints (before clipping)
float x0 = 1, y_0 = 1, z0 = 1; 
float x1 = 4, y_1 = 3, z1 = 2; 

// Draw the 3D clipping box using line segments
void drawBox() {
    float x[2] = { xmin, xmax };
    float y[2] = { ymin, ymax };
    float z[2] = { zmin, zmax };

    glColor3f(0.8f, 0.8f, 0.8f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j) {
        glVertex3f(x[0], y[i], z[j]); glVertex3f(x[1], y[i], z[j]);
        glVertex3f(x[i], y[0], z[j]); glVertex3f(x[i], y[1], z[j]);
        glVertex3f(x[i], y[j], z[0]); glVertex3f(x[i], y[j], z[1]);
    }
    glEnd();

    // Labels for box dimensions
    glColor3f(1, 1, 1);
    drawText(xmax + 0.1, ymin + 0.1, zmin, "Length: 5 cm");
    drawText(xmin + 0.1, ymax + 0.1, zmin, "Height: 4 cm");
    drawText(xmin + 0.1, ymin + 0.1, zmax + 0.1, "Width: 3 cm");
}

// Display callback: renders scene and performs clipping
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10, 8, 10, 2.5, 2, 1.5, 0, 1, 0);  // Set camera view
    glRotatef(angle, 0, 1, 0);  // Rotate scene for better 3D perspective

    // Draw coordinate axes
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(6, 0, 0); // X-axis
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0); // Y-axis
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 4); // Z-axis
    glEnd();

    drawBox();  // Draw clipping volume

    // Draw original line (in red)
    glColor3f(1, 0, 0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(x0, y_0, z0);
    glVertex3f(x1, y_1, z1);
    glEnd();

    // Label original line endpoints
    glColor3f(1, 1, 1);
    drawText(x0 + 0.1, y_0 + 0.1, z0, "P1 " + coordToStr(x0, y_0, z0));
    drawText(x1 + 0.1, y_1 + 0.1, z1, "P2 " + coordToStr(x1, y_1, z1));

    // Copy of endpoints for clipping (to preserve originals)
    float cx0 = x0, cy0 = y_0, cz0 = z0;
    float cx1 = x1, cy1 = y_1, cz1 = z1;

    // Perform clipping and draw clipped line
    if (cyrusBeckClip(cx0, cy0, cz0, cx1, cy1, cz1)) {
        // Draw clipped segment (cyan)
        glColor3f(0.0, 1.0, 1.0);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex3f(cx0, cy0, cz0);
        glVertex3f(cx1, cy1, cz1);
        glEnd();

        // Highlight clipped points (yellow)
        glPointSize(8);
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(cx0, cy0, cz0);
        glVertex3f(cx1, cy1, cz1);
        glEnd();

        // Label clipped endpoints
        glColor3f(0.9, 1.0, 0.9);
        drawText(cx0 + 0.1, cy0 + 0.1, cz0, "C1 " + coordToStr(cx0, cy0, cz0));
        drawText(cx1 + 0.1, cy1 + 0.1, cz1, "C2 " + coordToStr(cx1, cy1, cz1));
    }

    glutSwapBuffers();  // Swap front and back buffers for smooth animation
}

// Timer function for rotating the scene
void timer(int value) {
    angle += 0.5;  // Increment angle for rotation
    if (angle > 360) angle -= 360;
    glutPostRedisplay();  // Request display update
    glutTimerFunc(16, timer, 0);  // Re-register timer for ~60 FPS
}

// OpenGL initialization
void init() {
    glEnable(GL_DEPTH_TEST);  // Enable depth buffering
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1.0, 1.0, 50.0);  // Perspective projection
    glMatrixMode(GL_MODELVIEW);
}

// Main program entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(900, 800);
    glutCreateWindow("Cyrus-Beck 3D Clipping with Rotation and Coordinates");
    init();
    glutDisplayFunc(display);      // Register display callback
    glutTimerFunc(0, timer, 0);    // Start timer
    glutMainLoop();                // Enter event loop
    return 0;
}
