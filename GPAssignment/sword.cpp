#include <GL/glut.h>

float rotationX = 0.0f; // Rotation angle around the X-axis
float rotationY = 0.0f; // Rotation angle around the Y-axis

// Function to draw the 2D sword shape
void drawSword2D() {
    glBegin(GL_POLYGON);
    // Blade with gradient
    glColor3f(1.0f, 1.0f, 0.5f); // Gradient start (lighter gold)
    glVertex3f(-0.05f, -0.8f, 0.0f); // Bottom left
    glVertex3f(0.05f, -0.8f, 0.0f);  // Bottom right
    glColor3f(1.0f, 1.0f, 0.0f); // Gradient end (darker gold)
    glVertex3f(0.0f, 0.9f, 0.0f);    // Sharp tip
    glEnd();

    // Handle
    glBegin(GL_POLYGON);
    glColor3f(0.5f, 0.5f, 0.5f); // Silver color
    glVertex3f(-0.025f, -1.0f, 0.0f); // Bottom left (narrower handle)
    glVertex3f(0.025f, -1.0f, 0.0f);  // Bottom right (narrower handle)
    glVertex3f(0.025f, -0.5f, 0.0f);  // Top right
    glVertex3f(-0.025f, -0.5f, 0.0f); // Top left
    glEnd();

    // Crossguard
    glBegin(GL_POLYGON);
    glColor3f(0.8f, 0.8f, 0.0f); // Golden crossguard
    glVertex3f(-0.3f, -0.75f, 0.0f); // Bottom left
    glVertex3f(0.3f, -0.75f, 0.0f);  // Bottom right
    glVertex3f(0.2f, -0.7f, 0.0f);   // Top right
    glVertex3f(-0.2f, -0.7f, 0.0f);  // Top left
    glEnd();
}

// Function to draw the 3D sword by extruding 2D shape
void drawSword3D() {
    float depthStep = 0.01f; // Extrusion depth per step
    int steps = 8;         // Number of extrusion layers

    // Draw the extruded layers
    glPushMatrix();
    for (int i = 0; i <= steps; ++i) {
        float z = depthStep * (i - steps / 2.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, z);
        drawSword2D();
        glPopMatrix();
    }

    // Connect edges for 3D extrusion (blade)
    glBegin(GL_QUADS);
    glColor3f(0.8f, 0.7f, 0.0f); // Side color
    // Blade sides
    glVertex3f(-0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(0.05f, -0.8f, depthStep * steps / 2.0f);
    glVertex3f(-0.05f, -0.8f, depthStep * steps / 2.0f);

    glVertex3f(0.0f, 0.9f, -depthStep * steps / 2.0f);
    glVertex3f(-0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(-0.05f, -0.8f, depthStep * steps / 2.0f);
    glVertex3f(0.0f, 0.9f, depthStep * steps / 2.0f);

    glVertex3f(0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, 0.9f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, 0.9f, depthStep * steps / 2.0f);
    glVertex3f(0.05f, -0.8f, depthStep * steps / 2.0f);
    glEnd();

    // Top cap
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.05f, -0.8f, depthStep * steps / 2.0f);
    glVertex3f(0.05f, -0.8f, depthStep * steps / 2.0f);
    glVertex3f(0.0f, 0.9f, depthStep * steps / 2.0f);
    glEnd();

    // Bottom cap
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(0.05f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, 0.9f, -depthStep * steps / 2.0f);
    glEnd();

    // Handle extrusion
    glBegin(GL_QUADS);
    glColor3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.025f, -1.0f, -depthStep * steps / 2.0f);
    glVertex3f(0.025f, -1.0f, -depthStep * steps / 2.0f);
    glVertex3f(0.025f, -1.0f, depthStep * steps / 2.0f);
    glVertex3f(-0.025f, -1.0f, depthStep * steps / 2.0f);

    glVertex3f(-0.025f, -0.5f, -depthStep * steps / 2.0f);
    glVertex3f(0.025f, -0.5f, -depthStep * steps / 2.0f);
    glVertex3f(0.025f, -0.5f, depthStep * steps / 2.0f);
    glVertex3f(-0.025f, -0.5f, depthStep * steps / 2.0f);
    glEnd();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f); // Move back to view
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

    drawSword3D();

    glutSwapBuffers();
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f); // Dark background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// Keyboard input handler for rotation
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w':
            rotationX -= 5.0f;
            break;
        case 's':
            rotationX += 5.0f;
            break;
        case 'a':
            rotationY -= 5.0f;
            break;
        case 'd':
            rotationY += 5.0f;
            break;
        case 27: // Escape key
            exit(0);
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutCreateWindow("3D Sword");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
