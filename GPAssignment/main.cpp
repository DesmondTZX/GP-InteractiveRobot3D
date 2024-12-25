#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

int viewingMode = 0;

// Camera parameters
bool cameraView = false;
float camX = 0.0f, camY = 0.0f, camZ = 5.0f; // Camera position
float pitch = 0.0f, yaw = 0.0f;              // Camera rotation (angles in degrees)
const float moveStep = 0.1f;                 // Movement step size
const float sensitivity = 0.1f;             // Mouse sensitivity

// Previous mouse position
int lastMouseX = -1, lastMouseY = -1;

void handleKeys(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC key
        std::cout << "Exiting..." << std::endl;
        exit(0);

    case 9: // Tab key to toggle the camera view
        cameraView = !cameraView;
        if (cameraView) {
            std::cout << "Camera view is ON.\n";
        } else {
            // Reset camera transformations when toggling off
            camX = 0.0f;
            camY = 0.0f;
            camZ = 5.0f;
            pitch = 0.0f;
            yaw = 0.0f;
            lastMouseX = -1; // Reset mouse tracking
            lastMouseY = -1;
            std::cout << "Camera view is OFF. Camera reset to default.\n";
        }
        break;

    default:
        if (cameraView) {
            // Movement keys
            switch (key) {
            case 'w': camZ -= moveStep; break; // Move camera forward
            case 's': camZ += moveStep; break; // Move camera backward
            case 'a': camX -= moveStep; break; // Move camera left
            case 'd': camX += moveStep; break; // Move camera right
            case 'r': camY += moveStep; break; // Move camera up
            case 'f': camY -= moveStep; break; // Move camera down
            default: break;
            }
        }
        break;
    }

    glutPostRedisplay(); // Request a redraw of the scene
}

void handleMouseMotion(int x, int y) {
    if (!cameraView) {
        // Reset last mouse position to avoid unexpected jumps when cameraView is re-enabled
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    if (lastMouseX == -1 || lastMouseY == -1) {
        // Initialize last mouse position
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    // Calculate the difference in mouse movement
    int dx = x - lastMouseX;
    int dy = y - lastMouseY;

    // Update camera rotation based on mouse movement
    yaw += dx * sensitivity;
    pitch -= dy * sensitivity;

    // Clamp pitch to prevent flipping
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    // Update last mouse position
    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay(); // Request a redraw of the scene
}

void applyCameraTransformations() {
    // Rotate the camera based on pitch (X-axis) and yaw (Y-axis)
    glRotatef(-pitch, 1.0f, 0.0f, 0.0f); // Pitch (look up/down)
    glRotatef(-yaw, 0.0f, 1.0f, 0.0f);   // Yaw (look left/right)

    // Translate the camera position
    glTranslatef(-camX, -camY, -camZ);
}

void applyDefaultView() {
    // Reset the view to be centered in the middle of the screen, looking straight ahead
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f); // Default camera position: centered and pulled back slightly
    // No rotation, looking directly at the origin (0, 0, 0)
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    if (cameraView) {
        applyCameraTransformations(); // Use interactive camera
    } else {
        applyDefaultView(); // Use fixed default view
    }

    // Render a simple object (a cube here)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.0f);

    // placeholder cube, will replace with robot here
    glutSolidCube(1.0);
    glPopMatrix();

    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                                           // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);        // Set display mode
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                 // Set window size
    glutCreateWindow("BACS2173 - Assignment");          // Create window with title

	glEnable(GL_DEPTH_TEST); // Enable depth testing

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(handleKeys);
    // glutSpecialFunc(handleSpecialKeys);
    glutMotionFunc(handleMouseMotion);            // Register the mouse motion handler
    glutMainLoop();                               // Enter main event loop

    return 0;
}
