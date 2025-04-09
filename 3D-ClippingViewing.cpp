#include <GL/glut.h>
#include <string>
#include <vector>
#include <cmath>

// Global variables
float angle_x = 0.0f, angle_y = 0.0f;
float camera_z = 15.0f;
bool show_frustum = true;
float near_plane = 2.0f, far_plane = 50.0f;

struct Object {
    float x, y, z;
    float size;
    float color[3];
};

std::vector<Object> objects = {
    {0, 0, 0, 1.0f, {1.0f, 0.0f, 0.0f}},    // Center cube (red)
    {3, 2, -10, 0.8f, {0.0f, 1.0f, 0.0f}},  // Far green cube
    {-2, -1, -5, 0.6f, {0.0f, 0.0f, 1.0f}}, // Mid blue cube
    {1, 1.5f, -2, 0.4f, {1.0f, 1.0f, 0.0f}}, // Near yellow cube
    {0, 0, -far_plane*0.9f, 0.5f, {1.0f, 0.0f, 1.0f}} // Almost clipped cube (magenta)
};

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float light_pos[] = {1.0f, 1.0f, 1.0f, 0.0f};
    float light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
}

void draw_cube(float x, float y, float z, float size, const float color[3]) {
    glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(size, size, size);
    
    glBegin(GL_QUADS);
    glColor3fv(color);
    
    // Front face
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Back face
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Top face
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Bottom face
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Right face
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Left face
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();
    glPopMatrix();
}

void draw_frustum() {
    if (!show_frustum) return;
        
    glDisable(GL_LIGHTING);
    glColor3f(0.7f, 0.7f, 0.7f);  // Gray lines
    
    // Near plane (highlighted in green)
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -near_plane);
    glVertex3f( 1.0f, -1.0f, -near_plane);
    glVertex3f( 1.0f,  1.0f, -near_plane);
    glVertex3f(-1.0f,  1.0f, -near_plane);
    glEnd();
    
    // Far plane (highlighted in red)
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
    glVertex3f(-1.0f, -1.0f, -near_plane);
    glVertex3f(-5.0f, -5.0f, -far_plane);
    
    glVertex3f( 1.0f, -1.0f, -near_plane);
    glVertex3f( 5.0f, -5.0f, -far_plane);
    
    glVertex3f( 1.0f,  1.0f, -near_plane);
    glVertex3f( 5.0f,  5.0f, -far_plane);
    
    glVertex3f(-1.0f,  1.0f, -near_plane);
    glVertex3f(-5.0f,  5.0f, -far_plane);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

void draw_text(float x, float y, const std::string &text) {
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 600, 0, 600);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1.0, near_plane, far_plane);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, camera_z, 0, 0, 0, 0, 1, 0);

    // Rotate the scene
    glRotatef(angle_x, 1.0f, 0.0f, 0.0f);
    glRotatef(angle_y, 0.0f, 1.0f, 0.0f);

    // Draw all objects
    for (const auto &obj : objects) {
        draw_cube(obj.x, obj.y, obj.z, obj.size, obj.color);
    }

    // Draw frustum
    draw_frustum();
    
    // Display info text
    char info[100];
    snprintf(info, sizeof(info), "Camera Z: %.1f  Near: %.1f  Far: %.1f", camera_z, near_plane, far_plane);
    draw_text(10, 580, info);
    draw_text(10, 560, "Arrow keys: Rotate  W/S: Move camera  F: Toggle frustum");
    
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    key = tolower(key);
    if (key == 27) {  // ESC key
        exit(0);
    } else if (key == 'w') {
        camera_z -= 0.5f;  // Move camera forward
    } else if (key == 's') {
        camera_z += 0.5f;  // Move camera backward
    } else if (key == 'f') {
        show_frustum = !show_frustum;  // Toggle frustum visibility
    }
    glutPostRedisplay();
}

void special_keys(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_UP:    angle_x += 5; break;
        case GLUT_KEY_DOWN:  angle_x -= 5; break;
        case GLUT_KEY_LEFT:  angle_y += 5; break;
        case GLUT_KEY_RIGHT: angle_y -= 5; break;
    }
    glutPostRedisplay();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Enhanced 3D Clipping & Viewing Demonstration (C++)");
    
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keys);
    glutReshapeFunc(reshape);
    glutMainLoop();
    
    return 0;
}