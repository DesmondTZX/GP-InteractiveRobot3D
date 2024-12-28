#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES

#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

int viewingMode = 0;

GLUquadricObj* cyObj = NULL;
GLUquadricObj* spObj = NULL;
GLUquadricObj* dkObj = NULL;

GLuint innerbody;

// Camera parameters
bool cameraView = false;
bool isOrtho = false;
bool coordLines = false;
bool drawMode = false;

float camX = 0.0f, camY = 0.0f, camZ = 5.0f; // Camera position
float pitch = 0.0f, yaw = 0.0f;              // Camera rotation (angles in degrees)
const float moveStep = 0.1f;                 // Movement step size
const float sensitivity = 0.1f;             // Mouse sensitivity

// Previous mouse position
int lastMouseX = -1, lastMouseY = -1;

// Global variable to track mouse state
bool isMousePressed = false;

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
    
    case 8: // backspace to toggle viewport
        isOrtho = !isOrtho;
        break;

    case 32: // space key
        coordLines = !coordLines;
        break;

    case 91: // [ key
        drawMode = !drawMode;
        break;

    default:
        if (cameraView) {
            // Movement keys
            switch (key) {
            case 'w': camY += moveStep; break; // Move camera up
            case 's': camY -= moveStep; break; // Move camera down
            case 'a': camX -= moveStep; break; // Move camera left
            case 'd': camX += moveStep; break; // Move camera right
            default: break;
            }
        }
        break;
    }

    glutPostRedisplay(); // Request a redraw of the scene
}

// Function to render bitmap text
void renderBitmapString(float x, float y, void* font, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(font, c);
    }
}

GLuint loadTexture(const char *filename) {
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 3);
    if (!data) {
        std::cerr << "Error: Unable to load texture file!" << std::endl;
        return 0;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data); // Free the loaded image memory
    return textureID;
}

void handleMouse(int button, int state, int x, int y) {
    if (cameraView) {
        if (button == 3) { // Scroll up (zoom in)
            camX *= 0.95f; // Scale down X
            camY *= 0.95f; // Scale down Y
            camZ *= 0.95f; // Scale down Z
        } else if (button == 4) { // Scroll down (zoom out)
            camX *= 1.05f; // Scale up X
            camY *= 1.05f; // Scale up Y
            camZ *= 1.05f; // Scale up Z
        }
    }

    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isMousePressed = true;
            lastMouseX = x;
            lastMouseY = y; // Initialize position to avoid jump
        } else if (state == GLUT_UP) {
            isMousePressed = false;
            lastMouseX = -1; // Reset position
            lastMouseY = -1;
        }
    }

    glutPostRedisplay(); // Request a redraw
}

// Function to handle mouse motion
void handleMouseMotion(int x, int y) {
    if (!cameraView || !isMousePressed) {
        return; // Only update rotation if cameraView is active and mouse is pressed
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

void drawInnerHead() {
    glPushMatrix();
        glTranslatef(.0f, 1.2f, .0f);
        glRotatef(90.0f, 1.0f, .0f, .0f);
        glRotatef(45.0f, .0f, .0f, 1.0f);

        // Draw the cylinder
        gluCylinder(cyObj, .2, .2, .3, 4, 8);

        // Draw the bottom cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f); // Bottom of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1); // Disk with bottom radius of .2
        glPopMatrix();

        // Draw the top cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, .3f); // Top of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1); // Disk with top radius of .2
        glPopMatrix();
    glPopMatrix();
}

void drawOuterHeadFirstLyr() {
    // todo: add materials and normals for texturing/coloring
    glPushAttrib(GL_CURRENT_BIT);
        glColor3f(.0f, .0f, 1.0f);

        glPushMatrix();
            glTranslatef(.0f, .8f, .0f);

            glBegin(GL_QUADS);
                // front left
                glVertex3f(.0f, .75f, .0f);
                glVertex3f(-.5f, .5f, .0f);
                glVertex3f(-.5f, .0f, .0f);
                glVertex3f(.0f, .0f, .0f);
                
                // front right
                glVertex3f(.0f, .75f, .0f);
                glVertex3f(.5f, .5f, .0f);
                glVertex3f(.5f, .0f, .0f);
                glVertex3f(.0f, .0f, .0f);
                
                // back left
                glVertex3f(.0f, .75f, .5f);
                glVertex3f(-.5f, .5f, .5f);
                glVertex3f(-.5f, .0f, .5f);
                glVertex3f(.0f, .0f, .5f);
                
                // back right
                glVertex3f(.0f, .75f, .5f);
                glVertex3f(.5f, .5f, .5f);
                glVertex3f(.5f, .0f, .5f);
                glVertex3f(.0f, .0f, .5f);

                // left upper
                glVertex3f(.0f, .75f, .0f);
                glVertex3f(.0f, .75f, .5f);
                glVertex3f(-.5f, .5f, .5f);
                glVertex3f(-.5f, .5f, .0f);

                // left lower
                glVertex3f(-.5f, .5f, .0f);
                glVertex3f(-.5f, .5f, .5f);
                glVertex3f(-.5f, .0f, .5f);
                glVertex3f(-.5f, .0f, .0f);

                // right upper
                glVertex3f(.0f, .75f, .0f);
                glVertex3f(.0f, .75f, .5f);
                glVertex3f(.5f, .5f, .5f);
                glVertex3f(.5f, .5f, .0f);

                // right lower
                glVertex3f(.5f, .5f, .0f);
                glVertex3f(.5f, .5f, .5f);
                glVertex3f(.5f, .0f, .5f);
                glVertex3f(.5f, .0f, .0f);
            glEnd();
        glPopMatrix();
    glPopAttrib();
}

void drawEyes() {
    glPushAttrib(GL_CURRENT_BIT);
        glColor3f(1.0f, .0f, .0f);

        glPushMatrix();
            glTranslatef(.0f, .6f, .0f);

            glBegin(GL_LINES);
                glVertex3f(-.2f, .5f, .51f);
                glVertex3f(-.4f, .5f, .51f);
                
                glVertex3f(.2f, .5f, .51f);
                glVertex3f(.4f, .5f, .51f);
            glEnd();
        glPopMatrix();
    glPopAttrib();
}

void drawOuterHeadSecondLyr() {
    // todo: add materials and normals for texturing/coloring
    glPushAttrib(GL_CURRENT_BIT);
        glColor3f(1.0f, .65f, .0f);

        glPushMatrix();
            glTranslatef(.0f, .6f, .0f);

            glBegin(GL_QUADS);
                // upper left cube
                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, 1.0f, .4f);
                glVertex3f(-.5f, .75f, .4f);
                glVertex3f(-.5f, .75f, .2f);
                
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(-.5f, .7f, .4f);
                glVertex3f(-.5f, .7f, .2f);

                glVertex3f(.0f, 1.0f, .4f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(-.5f, .7f, .4f);
                glVertex3f(-.5f, .75f, .4f);
                
                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(-.5f, .7f, .2f);
                glVertex3f(-.5f, .75f, .2f);

                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(.0f, 1.0f, .4f);

                glVertex3f(-.5f, .75f, .2f);
                glVertex3f(-.5f, .7f, .2f);
                glVertex3f(-.5f, .7f, .4f);
                glVertex3f(-.5f, .75f, .4f);

                // lower left cube
                glVertex3f(-.5f, .75f, .2f);
                glVertex3f(-.5f, .2f, .2f);
                glVertex3f(-.5f, .2f, .4f);
                glVertex3f(-.5f, .75f, .4f);
                
                glVertex3f(-.55f, .75f, .2f);
                glVertex3f(-.55f, .2f, .2f);
                glVertex3f(-.55f, .2f, .4f);
                glVertex3f(-.55f, .75f, .4f);
                
                glVertex3f(-.55f, .75f, .2f);
                glVertex3f(-.5f, .75f, .2f);
                glVertex3f(-.5f, .2f, .2f);
                glVertex3f(-.55f, .2f, .2f);
                
                glVertex3f(-.55f, .75f, .4f);
                glVertex3f(-.5f, .75f, .4f);
                glVertex3f(-.5f, .2f, .4f);
                glVertex3f(-.55f, .2f, .4f);
                
                glVertex3f(-.55f, .75f, .4f);
                glVertex3f(-.5f, .75f, .4f);
                glVertex3f(-.5f, .75f, .2f);
                glVertex3f(-.55f, .75f, .2f);
                
                glVertex3f(-.55f, .2f, .4f);
                glVertex3f(-.5f, .2f, .4f);
                glVertex3f(-.5f, .2f, .2f);
                glVertex3f(-.55f, .2f, .2f);

                // upper right cube
                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, 1.0f, .4f);
                glVertex3f(.5f, .75f, .4f);
                glVertex3f(.5f, .75f, .2f);
                
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(.5f, .7f, .4f);
                glVertex3f(.5f, .7f, .2f);

                glVertex3f(.0f, 1.0f, .4f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(.5f, .7f, .4f);
                glVertex3f(.5f, .75f, .4f);
                
                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(.5f, .7f, .2f);
                glVertex3f(.5f, .75f, .2f);

                glVertex3f(.0f, 1.0f, .2f);
                glVertex3f(.0f, .95f, .2f);
                glVertex3f(.0f, .95f, .4f);
                glVertex3f(.0f, 1.0f, .4f);

                glVertex3f(.5f, .75f, .2f);
                glVertex3f(.5f, .7f, .2f);
                glVertex3f(.5f, .7f, .4f);
                glVertex3f(.5f, .75f, .4f);

                // lower right cube
                glVertex3f(.5f, .75f, .2f);
                glVertex3f(.5f, .2f, .2f);
                glVertex3f(.5f, .2f, .4f);
                glVertex3f(.5f, .75f, .4f);
                
                glVertex3f(.55f, .75f, .2f);
                glVertex3f(.55f, .2f, .2f);
                glVertex3f(.55f, .2f, .4f);
                glVertex3f(.55f, .75f, .4f);
                
                glVertex3f(.55f, .75f, .2f);
                glVertex3f(.5f, .75f, .2f);
                glVertex3f(.5f, .2f, .2f);
                glVertex3f(.55f, .2f, .2f);
                
                glVertex3f(.55f, .75f, .4f);
                glVertex3f(.5f, .75f, .4f);
                glVertex3f(.5f, .2f, .4f);
                glVertex3f(.55f, .2f, .4f);
                
                glVertex3f(.55f, .75f, .4f);
                glVertex3f(.5f, .75f, .4f);
                glVertex3f(.5f, .75f, .2f);
                glVertex3f(.55f, .75f, .2f);
                
                glVertex3f(.55f, .2f, .4f);
                glVertex3f(.5f, .2f, .4f);
                glVertex3f(.5f, .2f, .2f);
                glVertex3f(.55f, .2f, .2f);
            glEnd();
        glPopMatrix();
    glPopAttrib();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glLineWidth(1.0f);

    // cyObj
    cyObj = gluNewQuadric();

    spObj = gluNewQuadric();

    dkObj = gluNewQuadric();

    if (cameraView) {
        applyCameraTransformations(); // Use interactive camera
        renderBitmapString(-2.0f, 1.8f, GLUT_BITMAP_HELVETICA_18, "Camera Movement: ON");
    } else {
        applyDefaultView(); // Use fixed default view
        renderBitmapString(-2.0f, 1.8f, GLUT_BITMAP_HELVETICA_18, "Camera Movement: OFF");
    }

    if (coordLines) {
        renderBitmapString(-2.0f, 1.65f, GLUT_BITMAP_HELVETICA_18, "Coordinate Lines: ON");

        glBegin(GL_LINES);
            glVertex2f(-WINDOW_WIDTH, .0f);
            glVertex2f(WINDOW_WIDTH, .0f);
            
            glVertex2f(.0f, -WINDOW_HEIGHT);
            glVertex2f(.0f, WINDOW_HEIGHT);
        glEnd();
    } else {
        renderBitmapString(-2.0f, 1.65f, GLUT_BITMAP_HELVETICA_18, "Coordinate Lines: OFF");
    }

    if (isOrtho) {
        renderBitmapString(-2.0f, 1.5f, GLUT_BITMAP_HELVETICA_18, "Viewport: Orthogonal View");
    } else {
        renderBitmapString(-2.0f, 1.5f, GLUT_BITMAP_HELVETICA_18, "Viewport: Perspective View");
    }

    if (drawMode) {
        renderBitmapString(-2.0f, 1.35f, GLUT_BITMAP_HELVETICA_18, "Draw Mode: Line");
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        renderBitmapString(-2.0f, 1.35f, GLUT_BITMAP_HELVETICA_18, "Draw Mode: Fill");
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // enable texturing and auto assign normals for quadric objects
    // gluQuadricTexture(cyObj, GL_TRUE);
    // gluQuadricNormals(cyObj, GLU_SMOOTH);

    // bind texture to object
    // glBindTexture(GL_TEXTURE_2D, innerbody);

    
    // Inner left leg
    glPushMatrix();
        glTranslatef(-.3f, -.201f, .0f);
        glRotatef(90.0f, 1.0f, .0f, .0f);
        glRotatef(45.0f, .0f, .0f, 1.0f);

        // Draw the cylinder
        gluCylinder(cyObj, .2, .2, 1.5, 4, 8);

        // Draw the bottom cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f); // Bottom of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1); // Disk matches the radius of the cylinder
        glPopMatrix();

        // Draw the top cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.5f); // Top of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1);
        glPopMatrix();
    glPopMatrix();

    // Inner right leg
    glPushMatrix();
        glTranslatef(.3f, -.201f, .0f);
        glRotatef(90.0f, 1.0f, .0f, .0f);
        glRotatef(45.0f, .0f, .0f, 1.0f);

        // Draw the cylinder
        gluCylinder(cyObj, .2, .2, 1.5, 4, 8);

        // Draw the bottom cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f); // Bottom of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1);
        glPopMatrix();

        // Draw the top cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 1.5f); // Top of the cylinder
            gluDisk(dkObj, 0.0, .2, 4, 1);
        glPopMatrix();
    glPopMatrix();

    // Inner torso
    glPushMatrix();
        glTranslatef(.0f, -.3f, .0f);
        glRotatef(-90.0f, 1.0f, .0f, .0f);
        glRotatef(45.0f, .0f, .0f, 1.0f);

        // Draw the cylinder
        gluCylinder(cyObj, .35, .5, .85, 4, 8);

        // Draw the bottom cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f); // Bottom of the cylinder
            gluDisk(dkObj, 0.0, .35, 4, 1); // Disk with bottom radius of .35
        glPopMatrix();

        // Draw the top cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, .85f); // Top of the cylinder
            gluDisk(dkObj, 0.0, .5, 4, 1); // Disk with top radius of .5
        glPopMatrix();
    glPopMatrix();
    
    // Inner chest
    glPushMatrix();
        glTranslatef(.0f, .56f, .0f);
        glRotatef(-90.0f, 1.0f, .0f, .0f);
        glRotatef(90.0f, .0f, .0f, 1.0f);

        // Draw the cylinder
        gluCylinder(cyObj, .55, .85, .35, 6, 8);

        // Draw the bottom cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.0f); // Bottom of the cylinder
            gluDisk(dkObj, 0.0, .55, 6, 1); // Disk with bottom radius of .55
        glPopMatrix();

        // Draw the top cap
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, .35f); // Top of the cylinder
            gluDisk(dkObj, 0.0, .85, 6, 1); // Disk with top radius of .85
        glPopMatrix();
    glPopMatrix();

    // outer chest front (orange)
    glPushMatrix();

    glPopMatrix();

    // outer chest front (blue)
    glPushMatrix();

    glPopMatrix();

    
    // whole left arm
    glPushMatrix();
        // inner lower left arm
        glPushMatrix();
            glTranslatef(-1.3f, .8f, .0f);

            glRotatef(90.0f, .0f, .0f, 1.0f);
            glRotatef(90.0f, 1.0f, .0f, .0f);
            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, .8, 4, 8);
        glPopMatrix();

        // inner upper left arm
        glPushMatrix();
            glTranslatef(-1.3f, .8f, -.15f);

            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, 1.2, 4, 8);
        glPopMatrix();

        // inner left hand
        glPushMatrix();
            glTranslatef(-1.3f, .8f, 1.0f);

            glScalef(1.0, .5f, 1.0f);

            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, .4, 4, 8);
        glPopMatrix();

        // inner left fingers
    glPopMatrix();

    // whole right arm
    glPushMatrix();
        // inner lower right arm
        glPushMatrix();
            glTranslatef(.5f, .8f, .0f);

            glRotatef(90.0f, .0f, .0f, 1.0f);
            glRotatef(90.0f, 1.0f, .0f, .0f);
            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, .8, 4, 8);
        glPopMatrix();

        // inner upper right arm
        glPushMatrix();
            glTranslatef(1.3f, .8f, -.15f);

            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, 1.2, 4, 8);
        glPopMatrix();

        // inner right hand
        glPushMatrix();
            glTranslatef(1.3f, .8f, 1.0f);

            glScalef(1.0, .5f, 1.0f);

            glRotatef(45.0f, .0f, .0f, 1.0f);

            gluCylinder(cyObj, .2, .2, .4, 4, 8);
        glPopMatrix();

        // inner right fingers
    glPopMatrix();
    
    // Inner head
    drawInnerHead();

    glPushMatrix();
        glTranslatef(.0f, .29f, .0f);
        glScalef(.75f, .75f, .75f);

        // outer head 1st layer
        drawOuterHeadFirstLyr();

        // eyes
        glLineWidth(10.0f);
        drawEyes();

        glLineWidth(1.0f);

        // outer head 2nd layer
        drawOuterHeadSecondLyr();
    glPopMatrix();
    
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h); // Set the viewport to the new window size
    
    glMatrixMode(GL_PROJECTION); // Switch to the projection matrix
    glLoadIdentity(); // Reset the projection matrix

    if (isOrtho) {
        // Orthogonal projection: Ensure it adjusts for the window size.
        glOrtho(-1.0 * (double)w / (double)h, 1.0 * (double)w / (double)h, -1.0, 1.0, -1.0, 1.0);
    } else {
        // Perspective projection: Set the aspect ratio based on the new window size.
        gluPerspective(45.0f, (float)w / (float)h, .1, 100.0);
    }

    glMatrixMode(GL_MODELVIEW); // Switch back to the modelview matrix
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                                           // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);        // Set display mode
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                 // Set window size
    glutCreateWindow("BACS2173 - Assignment");          // Create window with title

	glEnable(GL_DEPTH_TEST); // Enable depth testing
    // glEnable(GL_TEXTURE_2D);

    innerbody = loadTexture("inner_body.bmp");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(handleKeys);
    // glutSpecialFunc(handleSpecialKeys);
    glutMouseFunc(handleMouse);
    glutMotionFunc(handleMouseMotion);
    glutMainLoop();                               // Enter main event loop

    return 0;
}
