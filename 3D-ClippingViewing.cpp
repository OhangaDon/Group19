#include <GL/glut.h>
#include <string>
#include <vector>
#include <cmath>

// Global camera and frustum parameters
float angle_x = 0.0f, angle_y = 0.0f; // Rotation angles for scene
float camera_z = 15.0f;               // Camera distance along z-axis
bool show_frustum = true;            // Toggle for showing the viewing frustum
float near_plane = 2.0f, far_plane = 50.0f; // Frustum clipping planes

// Structure representing a 3D cube object
struct Object {
    float x, y, z;      // Position
    float size;         // Uniform scale
    float color[3];     // RGB color
};

// Scene objects placed at varying depths and positions
std::vector<Object> objects = {
    {0, 0, 0, 1.0f, {1.0f, 0.0f, 0.0f}},    // Red cube at origin
    {3, 2, -10, 0.8f, {0.0f, 1.0f, 0.0f}},  // Green cube far back
    {-2, -1, -5, 0.6f, {0.0f, 0.0f, 1.0f}}, // Blue cube mid-range
    {1, 1.5f, -2, 0.4f, {1.0f, 1.0f, 0.0f}}, // Yellow cube close
    {0, 0, -far_plane * 0.9f, 0.5f, {1.0f, 0.0f, 1.0f}} // Magenta cube near far clipping
};

// Initial OpenGL state setup
void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Dark background
    glEnable(GL_DEPTH_TEST);             // Enable depth testing
    glEnable(GL_LIGHTING);               // Enable lighting
    glEnable(GL_LIGHT0);                 // Use light 0

    // Setup light properties
    float light_pos[] = {1.0f, 1.0f, 1.0f, 0.0f};     // Directional light
    float light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Bright white light
    float light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; // Soft ambient light

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    // Enable material coloring
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

// Draw a cube at specified location, size, and color
void draw_cube(float x, float y, float z, float size, const float color[3]) {
    glPushMatrix();
    glTranslatef(x, y, z);     // Position cube
    glScalef(size, size, size); // Scale cube

    glBegin(GL_QUADS);
    glColor3fv(color); // Apply color to all faces

    // Define all six faces of the cube with normals for lighting
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f); glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    
    glEnd();
    glPopMatrix();
}

// Draw the frustum lines for visualization
void draw_frustum() {
    if (!show_frustum) return;

    glDisable(GL_LIGHTING); // Disable lighting for clean lines

    // Draw near clipping plane (green)
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -near_plane);
    glVertex3f( 1.0f, -1.0f, -near_plane);
    glVertex3f( 1.0f,  1.0f, -near_plane);
    glVertex3f(-1.0f,  1.0f, -near_plane);
    glEnd();

    // Draw far clipping plane (red)
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5.0f, -5.0f, -far_plane);
    glVertex3f( 5.0f, -5.0f, -far_plane);
    glVertex3f( 5.0f,  5.0f, -far_plane);
    glVertex3f(-5.0f,  5.0f, -far_plane);
    glEnd();

    // Connect near and far planes
    glBegin(GL_LINES);
    glColor3f(0.7f, 0.7f, 0.7f);
    glVertex3f(-1.0f, -1.0f, -near_plane); glVertex3f(-5.0f, -5.0f, -far_plane);
    glVertex3f( 1.0f, -1.0f, -near_plane); glVertex3f( 5.0f, -5.0f, -far_plane);
    glVertex3f( 1.0f,  1.0f, -near_plane); glVertex3f( 5.0f,  5.0f, -far_plane);
    glVertex3f(-1.0f,  1.0f, -near_plane); glVertex3f(-5.0f,  5.0f, -far_plane);
    glEnd();

    glEnable(GL_LIGHTING); // Re-enable lighting
}

// Draw 2D text in screen space
void draw_text(float x, float y, const std::string &text) {
    glDisable(GL_LIGHTING);
    
    // Switch to 2D projection
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 600);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1, 1, 1); // White text
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }

    // Restore matrices
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glEnable(GL_LIGHTING);
}

// Main render function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Setup perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, near_plane, far_plane);

    // Set camera view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, camera_z, 0, 0, 0, 0, 1, 0);

    // Apply scene rotation
    glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

    // Draw cubes
    for (const auto &obj : objects) {
        draw_cube(obj.x, obj.y, obj.z, obj.size, obj.color);
    }

    // Draw the visual frustum
    draw_frustum();

    // Overlay controls and status text
    char info[100];
    snprintf(info, sizeof(info), "Camera Z: %.1f  Near: %.1f  Far: %.1f", camera_z, near_plane, far_plane);
    draw_text(10, 580, info);
    draw_text(10, 560, "Arrow keys: Rotate  W/S: Move camera  F: Toggle frustum");

    glutSwapBuffers();
}

// Handle keyboard input for camera and toggling
void keyboard(unsigned char key, int x, int y) {
    key = tolower(key);
    if (key == 27) { // ESC key
        exit(0);
    } else if (key == 'w') {
        camera_z -= 0.5f; // Move camera forward
    } else if (key == 's') {
        camera_z += 0.5f; // Move camera backward
    } else if (key == 'f') {
        show_frustum = !show_frustum; // Toggle frustum
    }
    glutPostRedisplay();
}

// Handle special arrow keys for rotation
void special_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    angle_x += 5; break;
        case GLUT_KEY_DOWN:  angle_x -= 5; break;
        case GLUT_KEY_LEFT:  angle_y += 5; break;
        case GLUT_KEY_RIGHT: angle_y -= 5; break;
    }
    glutPostRedisplay();
}

// Handle window resize
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

// Main entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Enhanced 3D Clipping & Viewing Demonstration (C++)");

    init(); // Setup OpenGL state
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutReshapeFunc(reshape);

    glutMainLoop(); // Enter event loop

    return 0;
}
