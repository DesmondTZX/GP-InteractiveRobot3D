#include <GL/glut.h>

float rotationX = 0.0f; // Rotation angle around the X-axis
float rotationY = 0.0f; // Rotation angle around the Y-axis

// Function to draw the 2D gun shape
void drawGun2D() {
    // Gun body
    glBegin(GL_POLYGON);
    glColor3f(1.0f, 0.84f, 0.0f); // Golden color
    glVertex3f(-0.6f, -0.2f, 0.0f); // Bottom left
    glVertex3f(0.6f, -0.2f, 0.0f);  // Bottom right
    glVertex3f(0.6f, 0.3f, 0.0f);   // Top right
    glVertex3f(0.3f, 0.3f, 0.0f);   // Transition
    glVertex3f(0.3f, 0.5f, 0.0f);   // Upper top
    glVertex3f(-0.6f, 0.5f, 0.0f);  // Top left
    glEnd();

    // Gun handle
    glBegin(GL_POLYGON);
    glColor3f(0.2f, 0.2f, 0.2f); // Black handle
    glVertex3f(-0.3f, -0.2f, 0.0f); // Bottom left
    glVertex3f(-0.1f, -0.2f, 0.0f); // Bottom right
    glVertex3f(-0.1f, -0.8f, 0.0f); // Top right
    glVertex3f(-0.3f, -0.8f, 0.0f); // Top left
    glEnd();

    // Gun trigger
    glBegin(GL_POLYGON);
    glColor3f(0.9f, 0.7f, 0.0f); // Golden trigger
    glVertex3f(-0.15f, -0.3f, 0.0f); // Top left
    glVertex3f(0.0f, -0.3f, 0.0f);  // Top right
    glVertex3f(0.0f, -0.4f, 0.0f);  // Bottom right
    glVertex3f(-0.15f, -0.5f, 0.0f); // Bottom left
    glEnd();
}

// Function to draw the 3D gun by extruding the 2D shape
void drawGun3D() {
    float depthStep = 0.01f; // Extrusion depth per step
    int steps = 15;         // Number of extrusion layers

    // Draw the extruded layers
    glPushMatrix();
    for (int i = 0; i <= steps; ++i) {
        float z = depthStep * (i - steps / 2.0f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, z);
        drawGun2D();
        glPopMatrix();
    }

    // Connect edges for 3D extrusion (gun body)
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.7f, 0.0f); // Side color

    // Front and back faces of the body
    glVertex3f(-0.6f, -0.2f, -depthStep * steps / 2.0f);
    glVertex3f(0.6f, -0.2f, -depthStep * steps / 2.0f);
    glVertex3f(0.6f, -0.2f, depthStep * steps / 2.0f);
    glVertex3f(-0.6f, -0.2f, depthStep * steps / 2.0f);

    glVertex3f(-0.6f, 0.5f, -depthStep * steps / 2.0f);
    glVertex3f(0.6f, 0.5f, -depthStep * steps / 2.0f);
    glVertex3f(0.6f, 0.5f, depthStep * steps / 2.0f);
    glVertex3f(-0.6f, 0.5f, depthStep * steps / 2.0f);
    glEnd();

    // Handle extrusion
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f);
    glVertex3f(-0.3f, -0.2f, -depthStep * steps / 2.0f);
    glVertex3f(-0.1f, -0.2f, -depthStep * steps / 2.0f);
    glVertex3f(-0.1f, -0.2f, depthStep * steps / 2.0f);
    glVertex3f(-0.3f, -0.2f, depthStep * steps / 2.0f);

    glVertex3f(-0.3f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(-0.1f, -0.8f, -depthStep * steps / 2.0f);
    glVertex3f(-0.1f, -0.8f, depthStep * steps / 2.0f);
    glVertex3f(-0.3f, -0.8f, depthStep * steps / 2.0f);
    glEnd();

    // Trigger extrusion
    glBegin(GL_QUADS);
    glColor3f(0.9f, 0.7f, 0.0f);
    glVertex3f(-0.15f, -0.3f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, -0.3f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, -0.3f, depthStep * steps / 2.0f);
    glVertex3f(-0.15f, -0.3f, depthStep * steps / 2.0f);

    glVertex3f(-0.15f, -0.5f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, -0.4f, -depthStep * steps / 2.0f);
    glVertex3f(0.0f, -0.4f, depthStep * steps / 2.0f);
    glVertex3f(-0.15f, -0.5f, depthStep * steps / 2.0f);
    glEnd();

    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f); // Move back to view
    glRotatef(rotationX, 1.0f, 0.0f, 0.0f); // Rotate around X-axis
    glRotatef(rotationY, 0.0f, 1.0f, 0.0f); // Rotate around Y-axis

    drawGun3D();

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
    glutCreateWindow("3D Gun");

    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}
