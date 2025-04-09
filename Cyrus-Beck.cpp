#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

// Box dimensions
const float xmin = 0.0f, xmax = 5.0f;
const float ymin = 0.0f, ymax = 4.0f;
const float zmin = 0.0f, zmax = 3.0f;

float angle = 0.0f;  // rotation angle

// Define the clipping planes (normal vectors and offsets)
struct Plane {
    float normal[3];
    float d;  // Distance from the origin
};

std::vector<Plane> planes = {
    {{1, 0, 0}, xmin},  // Left plane
    {{-1, 0, 0}, -xmax},  // Right plane
    {{0, 1, 0}, ymin},  // Bottom plane
    {{0, -1, 0}, -ymax},  // Top plane
    {{0, 0, 1}, zmin},  // Near plane
    {{0, 0, -1}, -zmax}  // Far plane
};

// Text rendering
void drawText(float x, float y, float z, std::string text, void *font = GLUT_BITMAP_HELVETICA_12) {
    glRasterPos3f(x, y, z);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

std::string coordToStr(float x, float y, float z) {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);
    ss << "(" << x << ", " << y << ", " << z << ")";
    return ss.str();
}

// Parametric line equation
void parametricLine(float t, float x0, float y0, float z0, float x1, float y1, float z1, float &x, float &y, float &z) {
    x = x0 + t * (x1 - x0);
    y = y0 + t * (y1 - y0);
    z = z0 + t * (z1 - z0);
}

// Dot product function
float dotProduct(const float a[3], const float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Cyrus-Beck 3D Clipping
bool cyrusBeckClip(float &x0, float &y0, float &z0, float &x1, float &y1, float &z1) {
    float t0 = 0.0f, t1 = 1.0f;  // Parametric values for the intersection
    bool accept = true;

    for (const auto &plane : planes) {
        float p0[3] = {x0, y0, z0};
        float p1[3] = {x1, y1, z1};
        float dir[3] = {p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2]};
        float denom = dotProduct(plane.normal, dir);

        if (denom > 0) {  // Line is entering the half-space
            float num = dotProduct(plane.normal, p0) - plane.d;
            float t = -num / denom;
            if (t > t0) t0 = t;  // Update near intersection point
        } else if (denom < 0) {  // Line is leaving the half-space
            float num = dotProduct(plane.normal, p0) - plane.d;
            float t = -num / denom;
            if (t < t1) t1 = t;  // Update far intersection point
        } else if (dotProduct(plane.normal, p0) - plane.d > 0) {
            // Line is outside the clipping region and parallel to the plane
            accept = false;
            break;
        }
    }

    // Clip the line if the parametric range is valid
    if (accept && t0 < t1) {
        parametricLine(t0, x0, y0, z0, x1, y1, z1, x0, y0, z0);
        parametricLine(t1, x0, y0, z0, x1, y1, z1, x1, y1, z1);
    }

    return accept;
}

// Original line endpoints
float x0 = -1, y_0 = 2, z0 = 1;
float x1 = 6, y_1 = 5, z1 = 4;

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

    // Labels for dimensions (Length, Width, Height)
    glColor3f(1, 1, 1);  // White color for labels
    // Length (X-axis)
    drawText(xmax + 0.1, ymin + 0.1, zmin, "Length: 5 cm");
    // Height (Y-axis)
    drawText(xmin + 0.1, ymax + 0.1, zmin, "Height: 4 cm");
    // Width (Z-axis)
    drawText(xmin + 0.1, ymin + 0.1, zmax + 0.1, "Width: 3 cm");
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(10, 8, 10, 2.5, 2, 1.5, 0, 1, 0);
    glRotatef(angle, 0, 1, 0);

    // Axes
    glLineWidth(1.5);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); glVertex3f(0, 0, 0); glVertex3f(6, 0, 0); // X - red
    glColor3f(0, 1, 0); glVertex3f(0, 0, 0); glVertex3f(0, 5, 0); // Y - green
    glColor3f(0, 0, 1); glVertex3f(0, 0, 0); glVertex3f(0, 0, 4); // Z - blue
    glEnd();

    drawBox();

    // Draw original line in red
    glColor3f(1, 0, 0);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3f(x0, y_0, z0);
    glVertex3f(x1, y_1, z1);
    glEnd();

    // Label original endpoints
    glColor3f(1, 1, 1);
    drawText(x0 + 0.1, y_0 + 0.1, z0, "P1 " + coordToStr(x0, y_0, z0));
    drawText(x1 + 0.1, y_1 + 0.1, z1, "P2 " + coordToStr(x1, y_1, z1));

    // Clipped region
    float cx0 = x0, cy0 = y_0, cz0 = z0;
    float cx1 = x1, cy1 = y_1, cz1 = z1;

    if (cyrusBeckClip(cx0, cy0, cz0, cx1, cy1, cz1)) {
        // Draw clipped segment in cyan
        glColor3f(0.0, 1.0, 1.0);
        glLineWidth(4.0);
        glBegin(GL_LINES);
        glVertex3f(cx0, cy0, cz0);
        glVertex3f(cx1, cy1, cz1);
        glEnd();

        // Mark clipped points
        glPointSize(8);
        glColor3f(1.0, 1.0, 0.0);
        glBegin(GL_POINTS);
        glVertex3f(cx0, cy0, cz0);
        glVertex3f(cx1, cy1, cz1);
        glEnd();

        // Coordinate labels
        glColor3f(0.9, 1.0, 0.9);
        drawText(cx0 + 0.1, cy0 + 0.1, cz0, "C1 " + coordToStr(cx0, cy0, cz0));
        drawText(cx1 + 0.1, cy1 + 0.1, cz1, "C2 " + coordToStr(cx1, cy1, cz1));
    }

    glutSwapBuffers();
}

void timer(int value) {
    angle += 0.5;
    if (angle > 360) angle -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(60, 1.0, 1.0, 50.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(900, 800);
    glutCreateWindow("Cyrus-Beck 3D Clipping with Rotation and Coordinates");
    init();
    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
