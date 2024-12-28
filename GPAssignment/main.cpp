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
bool drawMode = true;

float camX = 0.0f, camY = 0.0f, camZ = 8.0f; // Camera position
float pitch = 0.0f, yaw = 0.0f;              // Camera rotation (angles in degrees)
const float moveStep = 0.1f;                 // Movement step size
const float sensitivity = 0.1f;             // Mouse sensitivity

// Previous mouse position
int lastMouseX = -1, lastMouseY = -1;

// Global variable to track mouse state
bool isMousePressed = false;

// Arm Properties
float leftWholeArmRotateX = 90.0f;
float rightWholeArmRotateX = 90.0f;
float wholeArmRotateZ = 0.0f;
float lowerArmRotateX = 0.0f;
float lowerArmRotateY = 0.0f;
float rotateAngle = 5.0f;

// Finger Properties
float baseAngle = 0.0f;
float middleAngle = 0.0f;
float tipAngle = 0.0f;
const GLfloat BASE_ANGLE_MIN = 0.0f;
const GLfloat BASE_ANGLE_MAX = 90.0f;
const GLfloat MIDDLE_ANGLE_MIN = 0.0f;
const GLfloat MIDDLE_ANGLE_MAX = 105.0f;
const GLfloat TIP_ANGLE_MIN = 0.0f;
const GLfloat TIP_ANGLE_MAX = 120.0f;

// Walking and Running Animation
bool isWalking = false;
bool isRunning = false;
float animationSpeed = 0.2f; // Speed of the animation (walking is slower, running is faster)
float leftLegRotationAngle = 90.0f; // Rotation angle for legs
float rightLegRotationAngle = 90.0f; // Rotation angle for legs
bool legDirection = true; // Direction of leg rotation
bool armDirection = true; // Direction of arm rotation

// Shooting Animation
bool isShooting = false;
float weaponX = 0.0, weaponY = 0.0, weaponZ = 0.0;
float bulletSpeed = 0.01;
bool isGun = false;
BITMAP BMP;
HBITMAP hBMP = NULL;

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
            camZ = 8.0f;
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

            case 45: // minus key (not on numpad)
                camZ += moveStep;
                break;
            case 61: // equal key
                camZ -= moveStep;
                break;
            default: break;
            }
		}
    // Rotate the arm
	case '1':
        if (leftWholeArmRotateX > 0 && wholeArmRotateZ == 90) {
            leftWholeArmRotateX -= rotateAngle;
            rightWholeArmRotateX -= rotateAngle;
        }
		break;
	case '2':
        if (leftWholeArmRotateX < 90 && wholeArmRotateZ == 90) {
            leftWholeArmRotateX += rotateAngle;
            rightWholeArmRotateX += rotateAngle;
        }
		break;
	case '3':
		if (wholeArmRotateZ > 0 && leftWholeArmRotateX == 90) wholeArmRotateZ -= rotateAngle;
		break;
	case '4':
		if (wholeArmRotateZ < 90 && leftWholeArmRotateX == 90) wholeArmRotateZ += rotateAngle;
		break;
	case '5':
		if (lowerArmRotateX >= -90 && lowerArmRotateY == 0) lowerArmRotateX -= rotateAngle;
		break;
	case '6':
		if (lowerArmRotateX < 0 && lowerArmRotateY == 0) lowerArmRotateX += rotateAngle;
		break;
	case '7':
		if (lowerArmRotateY > -90 && lowerArmRotateX == 0) lowerArmRotateY -= rotateAngle;
		break;
	case '8':
		if (lowerArmRotateY < 90 && lowerArmRotateX == 0) lowerArmRotateY += rotateAngle;
		break;

        // Finger Bend
    case 'c':
        if (baseAngle < BASE_ANGLE_MAX) baseAngle += 1.0f;
        if (middleAngle < MIDDLE_ANGLE_MAX) middleAngle += 1.5f;
        if (tipAngle < TIP_ANGLE_MAX) tipAngle += 2.0f;
        break;

        // Finger Release
    case 'o':
        if (baseAngle > BASE_ANGLE_MIN) baseAngle -= 1.0f;
        if (middleAngle > MIDDLE_ANGLE_MIN) middleAngle -= 1.5f;
        if (tipAngle > TIP_ANGLE_MIN) tipAngle -= 2.0f;
        break;

    case 'n': // Start walking
        isWalking = true;
        isRunning = false;
        std::cout << "Walking animation started.\n";
        break;

    case 'm': // Start running
        isRunning = true;
        isWalking = false;
        std::cout << "Running animation started.\n";
        break;

    case 'x': // Stop animation
        isWalking = false;
        isRunning = false;
        std::cout << "Animation stopped.\n";
        break;

    case 'p': // Start shooting
		isShooting = true;
		std::cout << "Shooting animation started.\n";
		break;
    case 'l':
        weaponX = 0;
        weaponY = 0;
        weaponZ = 0;

		break;
    case 'r':
        // reset animation
        leftWholeArmRotateX = 90.0f;
        rightWholeArmRotateX = 90.0f;
        wholeArmRotateZ = 0.0f;
        lowerArmRotateX = 0.0f;
        lowerArmRotateY = 0.0f;
        rotateAngle = 5.0f;

        // Finger Properties
        baseAngle = 0.0f;
        middleAngle = 0.0f;
        tipAngle = 0.0f;

        // Walking and Running Animation
        isWalking = false;
        isRunning = false;
        animationSpeed = 0.2f; // Speed of the animation (walking is slower, running is faster)
        leftLegRotationAngle = 90.0f; // Rotation angle for legs
        rightLegRotationAngle = 90.0f; // Rotation angle for legs
        legDirection = true; // Direction of leg rotation
        armDirection = true; // Direction of arm rotation

        // Shooting Animation
        isShooting = false;
        weaponX = 0.0, weaponY = 0.0, weaponZ = 0.0;
        bulletSpeed = 0.01;
        isGun = false;
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

GLuint loadTexture2(LPCSTR filename) {
    GLuint texture = 0;

    // Convert LPCSTR to LPCWSTR
    int wchars_num = MultiByteToWideChar(CP_ACP, 0, filename, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_ACP, 0, filename, -1, wstr, wchars_num);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    HBITMAP hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
        wstr, IMAGE_BITMAP, 0, 0,
        LR_CREATEDIBSECTION | LR_LOADFROMFILE);

    delete[] wstr;  // Free the allocated memory

    if (!hBMP) {
        return 0;  // Return 0 if loading the image failed
    }

    BITMAP BMP;
    GetObject(hBMP, sizeof(BMP), &BMP);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

    DeleteObject(hBMP);

    return texture;
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
    glTranslatef(0.0f, 0.0f, -8.0f); // Default camera position: centered and pulled back slightly
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

void drawInnerChest() {
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
}

void drawOuterChest() {
    // outer chest front (orange)
    glPushAttrib(GL_CURRENT_BIT);
        glColor3f(1.0f, .65f, .0f);
    
        glPushMatrix();
            glTranslatef(.0f, .7f, .0f);
            glRotatef(-90.0f, 1.0f, .0f, .0f);
            glRotatef(90.0f, .0f, .0f, 1.0f);

            // Draw the cylinder
            gluCylinder(cyObj, .65, .95, .2, 6, 8);
        glPopMatrix();
    glPopAttrib();

    // outer chest front (blue)
    glPushAttrib(GL_CURRENT_BIT);
        glColor3f(.0f, .0f, 1.0f);

        glPushMatrix();
            glTranslatef(.0f, .6f, .0f);
            glRotatef(-90.0f, 1.0f, .0f, .0f);
            glRotatef(90.0f, .0f, .0f, 1.0f);

            // Draw the cylinder
            gluCylinder(cyObj, .65, .75, .15, 6, 8);
        glPopMatrix();
    glPopAttrib();
}

void drawFinger() {
    // Base segment
    gluCylinder(cyObj, 0.05, 0.05, 0.2, 8, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.2f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);  // Bend the finger slightly
    // Middle segment
    gluCylinder(cyObj, 0.04, 0.04, 0.15, 8, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.15f);
    glRotatef(20.0f, 1.0f, 0.0f, 0.0f);  // Further bend
    // Tip segment
    gluCylinder(cyObj, 0.03, 0.03, 0.1, 8, 8);
    glPopMatrix();
    glPopMatrix();
}

void drawFinger(GLfloat baseAngle, GLfloat middleAngle, GLfloat tipAngle) {
    // Base segment
    gluCylinder(cyObj, 0.05, 0.05, 0.2, 8, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.2f);
    glRotatef(baseAngle, 1.0f, 0.0f, 0.0f);  // Bend the base joint

    // Middle segment
    gluCylinder(cyObj, 0.04, 0.04, 0.15, 8, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.15f);
    glRotatef(middleAngle, 1.0f, 0.0f, 0.0f);  // Bend the middle joint

    // Tip segment
    gluCylinder(cyObj, 0.03, 0.03, 0.1, 8, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.1f);
    glRotatef(tipAngle, 1.0f, 0.0f, 0.0f);  // Bend the tip joint

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

void drawLeftArm() {
    // Whole left arm
    glRotatef(-wholeArmRotateZ, 0.0f, 0.0f, 1.0f); 
    glRotatef(leftWholeArmRotateX, 1.0f, 0.0f, 0.0f); // TODO: Rotate the hand like walking and running animation
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    gluCylinder(cyObj, 0.2, 0.2, 0.6, 4, 8);
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.4f);     
        glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);  // Counter-rotate
        glRotatef(lowerArmRotateX, 1.0f, 0.0f, 0.0f);  
        glRotatef(lowerArmRotateY, 0.0f, 1.0f, 0.0f);   
        glRotatef(45.0f, 0.0f, 0.0f, 1.0f);   // Restore rotation

        gluCylinder(cyObj, 0.2, 0.2, 0.6, 4, 8);

        // Palm
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.6f);
            gluCylinder(cyObj, 0.2, 0.2, 0.2, 4, 8);

            // Finger group
            glPushMatrix();
                glRotatef(135.0f, 0.0f, 0.0f, 1.0f);  // Tilt the fingers
                glTranslatef(0.0f, 0.0f, 0.15f);
    
                // Thumb
                glPushMatrix();
                    glScalef(0.8f, 0.8f, 0.8f);
                    glTranslatef(0.15f, 0.1f, -0.2f);
                    glRotatef(30.0f, 0.0f, 1.0f, 0.0f);  // Tilt the thumb
                    drawFinger(baseAngle, middleAngle, tipAngle);
                glPopMatrix();

                // Index finger
                glPushMatrix();
                    glScalef(0.7f, 0.7f, 0.7f);
                    glTranslatef(-0.1f, 0.1f, 0.0f);
                    drawFinger(baseAngle, middleAngle, tipAngle);
                glPopMatrix();

                // Middle finger
                glPushMatrix();
                    glScalef(0.7f, 0.7f, 0.7f);
                    glTranslatef(0.0f, 0.1f, 0.0f);
                    drawFinger(baseAngle, middleAngle, tipAngle);
                glPopMatrix();

                // Ring finger
                glPushMatrix();
                    glScalef(0.7f, 0.7f, 0.7f);
                    glTranslatef(0.1f, 0.1f, 0.0f);
                    drawFinger(baseAngle, middleAngle, tipAngle);
                glPopMatrix();

                // Little finger
                glPushMatrix();
                    glScalef(0.7f, 0.7f, 0.7f);
                    glTranslatef(0.2f, 0.1f, 0.0f);
                    drawFinger(baseAngle, middleAngle, tipAngle);
                glPopMatrix();

            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}

void drawRightArm() {
    // Whole right arm
    glRotatef(wholeArmRotateZ, 0.0f, 0.0f, 1.0f);
    glRotatef(rightWholeArmRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);

    gluCylinder(cyObj, 0.2, 0.2, 0.6, 4, 8);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.4f);
    glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);  // Counter-rotate
    glRotatef(lowerArmRotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(-lowerArmRotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(45.0f, 0.0f, 0.0f, 1.0f);   // Restore rotation

    gluCylinder(cyObj, 0.2, 0.2, 0.6, 4, 8);

    // Palm
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 0.6f);
    gluCylinder(cyObj, 0.2, 0.2, 0.2, 4, 8);

    // Finger group
    glPushMatrix();
    glRotatef(135.0f, 0.0f, 0.0f, 1.0f);  // Tilt the fingers
    glTranslatef(-0.05f, 0.0f, 0.15f);

    // Thumb
    glPushMatrix();
    glScalef(0.8f, 0.8f, 0.8f);
    glTranslatef(-0.1f, -0.1f, -0.2f);
    glRotatef(-30.0f, 0.0f, 1.0f, 0.0f);  // Tilt the thumb
    drawFinger(baseAngle, middleAngle, tipAngle);
    glPopMatrix();

    // Index finger
    glPushMatrix();
    glScalef(0.7f, 0.7f, 0.7f);
    glTranslatef(-0.1f, 0.1f, 0.0f);
    drawFinger(baseAngle, middleAngle, tipAngle);
    glPopMatrix();

    // Middle finger
    glPushMatrix();
    glScalef(0.7f, 0.7f, 0.7f);
    glTranslatef(0.0f, 0.1f, 0.0f);
    drawFinger(baseAngle, middleAngle, tipAngle);
    glPopMatrix();

    // Ring finger
    glPushMatrix();
    glScalef(0.7f, 0.7f, 0.7f);
    glTranslatef(0.1f, 0.1f, 0.0f);
    drawFinger(baseAngle, middleAngle, tipAngle);
    glPopMatrix();

    // Little finger
    glPushMatrix();
    glScalef(0.7f, 0.7f, 0.7f);
    glTranslatef(0.2f, 0.1f, 0.0f);
    drawFinger(baseAngle, middleAngle, tipAngle);
    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

void sickleStick(double tr, double br, double h) {
    GLUquadricObj* cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, true);
    gluCylinder(cylinder, tr, br, h, 20, 20);
    gluDeleteQuadric(cylinder);
}

void sickleCorn(double tr, double br, double h) {
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadricObj* cylinder = NULL;
    cylinder = gluNewQuadric();
    gluQuadricDrawStyle(cylinder, GLU_FILL);
    gluQuadricTexture(cylinder, true);
    gluCylinder(cylinder, tr, br, h, 10, 10);
    gluDeleteQuadric(cylinder);
}

void sickleBlade(float sz, float sh) {
    glBegin(GL_QUADS);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0, 0.0, sh);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.15, 0.0, sh);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.15, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.15 / 2, sz, sh / 2);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 0.0, sh);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.15 / 2, sz, sh / 2);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.15 / 2, sz, sh / 2);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.15, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.15 / 2, sz, sh / 2);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.15 / 2, sz, sh / 2);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.15, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.15, 0.0, sh);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.15 / 2, sz, sh / 2);

    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.15 / 2, sz, sh / 2);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.15, 0.0, sh);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.0, sh);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.15 / 2, sz, sh / 2);
    glEnd();
}

void pistolCube(float sizeX, float sizeY, float sizeZ) {
    glBegin(GL_QUADS);
    //left
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, sizeY, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, sizeY, sizeZ);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, sizeZ);

    //top
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0, 0.0, sizeZ);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(sizeX, 0.0, sizeZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, sizeY, sizeZ);

    //back
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0, sizeY, sizeZ);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(sizeX, sizeY, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, sizeY, 0.0);

    //bottom
    glTexCoord2f(0.0, 1.0);
    glVertex3f(0.0, sizeY, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(sizeX, 0.0, 0.0);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(sizeX, sizeY, 0.0);

    //right
    glTexCoord2f(0.0, 1.0);
    glVertex3f(sizeX, sizeY, 0.0f);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(sizeX, sizeY, sizeZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(sizeX, 0.0, sizeZ);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(sizeX, 0.0, 0.0);

    //front
    glTexCoord2f(0.0, 1.0);
    glVertex3f(sizeX, 0.0, 0.0);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(sizeX, 0.0, sizeZ);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(0.0, 0.0, sizeZ);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();
}

void pistolLine(float sizeX, float sizeY, float sizeZ) {
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, sizeY, 0.0);
    glVertex3f(0.0, sizeY, sizeZ);
    glVertex3f(0.0, 0.0, sizeZ);

    glVertex3f(0.0, 0.0, sizeZ);
    glVertex3f(sizeX, 0.0, sizeZ);
    glVertex3f(sizeX, sizeY, sizeZ);
    glVertex3f(0.0, sizeY, sizeZ);

    glVertex3f(0.0, sizeY, sizeZ);
    glVertex3f(sizeX, sizeY, sizeZ);
    glVertex3f(sizeX, sizeY, 0.0);
    glVertex3f(0.0, sizeY, 0.0);

    glVertex3f(0.0, sizeY, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(sizeX, 0.0, 0.0);
    glVertex3f(sizeX, sizeY, 0.0);

    glVertex3f(sizeX, sizeY, 0.0f);
    glVertex3f(sizeX, sizeY, sizeZ);
    glVertex3f(sizeX, 0.0, sizeZ);
    glVertex3f(sizeX, 0.0, 0.0);

    glVertex3f(sizeX, 0.0, 0.0);
    glVertex3f(sizeX, 0.0, sizeZ);
    glVertex3f(0.0, 0.0, sizeZ);
    glVertex3f(0.0, 0.0, 0.0);
    glEnd();
}

void pistolHole() {
    float x = 0.0, y = 0.0, x2 = 0, y2 = 0, r = 0.02, angle = 0, PI = 3.1429, noOfTri = 60;

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0, 0.0, 0.0);
    for (angle = 0; angle <= (2 * PI); angle += (2 * PI) / noOfTri) {
        x2 = x + r * cos(angle);
        y2 = y + r * sin(angle);
        glVertex2f(x2, y2);
        glColor3f(0.35, 0.35, 0.33);
    }
    glEnd();
}

void bullet() {
    glPushMatrix();
    glRotatef(90, 0.0, 1.0, 0.0);
    glTranslatef(-0.1, 0.0, 0.1);
    sickleStick(0.01, 0.01, 0.05);
    glPopMatrix();

    glPushMatrix();
    glRotatef(90, 0.0, 0.0, 1.0);
    glTranslatef(0.0, -0.15, 0.0);
    sickleCorn(0.01, 0.0, 0.05);
    glPopMatrix();
}


void pistol() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glRotatef(-90, 1.0, 0.0, 0.0);
    GLuint textureArr[2];

    //pistol body
    glPushMatrix();
    textureArr[0] = loadTexture2("pistolsilver.bmp");
    glColor3f(0.83, 0.83, 0.83);
    pistolCube(0.35, 0.1, 0.1);
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.35, 0.1, 0.1);
    glPopMatrix();

    //pistol handle
    glPushMatrix();
    textureArr[1] = loadTexture2("brownleather.bmp");
    glTranslatef(0.05, 0.01, 0.05);
    glRotatef(110, 0.0, 1.0, 0.0);
    glColor3f(0.64, 0.66, 0.69);
    pistolCube(0.2, 0.08, 0.08);
    glDeleteTextures(1, &textureArr[1]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.05, 0.01, 0.05);
    glRotatef(110, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.2, 0.08, 0.08);
    glPopMatrix();

    //pistol scope
    glPushMatrix();
    textureArr[0] = loadTexture2("pistolsilver.bmp");
    glTranslatef(0.3, 0.04, 0.01);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(0.64, 0.66, 0.69);
    pistolCube(0.12, 0.03, 0.03);
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.3, 0.04, 0.01);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.12, 0.03, 0.03);
    glPopMatrix();

    //pistol shooting hole
    glPushMatrix();
    glTranslatef(0.355, 0.05, 0.05);
    glRotatef(90, 0.0, 1.0, 0.0);
    pistolHole();
    glPopMatrix();

    //pistol press space straight
    glPushMatrix();
    textureArr[0] = loadTexture2("pistolsilver.bmp");
    glTranslatef(0.2, 0.03, -0.05);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(0.64, 0.66, 0.69);
    pistolCube(0.12, 0.04, 0.02);
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2, 0.03, -0.05);
    glRotatef(270, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.12, 0.04, 0.02);
    glPopMatrix();

    //pistol press space horizontal
    glPushMatrix();
    textureArr[0] = loadTexture2("pistolsilver.bmp");
    glTranslatef(0.2, 0.03, -0.05);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.64, 0.66, 0.69);
    pistolCube(0.12, 0.04, 0.02);
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.2, 0.03, -0.05);
    glRotatef(180, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.12, 0.04, 0.02);
    glPopMatrix();

    //pistol shooting button
    glPushMatrix();
    textureArr[0] = loadTexture2("pistolsilver.bmp");
    glTranslatef(0.16, 0.043, -0.025);
    glRotatef(230, 0.0, 1.0, 0.0);
    glColor3f(0.85, 0.85, 0.85);
    pistolCube(0.08, 0.02, 0.02);
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.16, 0.043, -0.025);
    glRotatef(230, 0.0, 1.0, 0.0);
    glColor3f(0.0, 0.0, 0.0);
    pistolLine(0.08, 0.02, 0.02);
    glPopMatrix();
}

void bulletShoot() {
    if (weaponX > 1.0) {
        weaponX = 1.01;
    }
    else {
        weaponX = weaponX + bulletSpeed;
    }

    glTranslatef(weaponX, weaponY, weaponZ);

    glPushMatrix();
    GLuint textureArr[1];
    textureArr[0] = loadTexture2("bullet.bmp");
    glTranslatef(0.3, 0.05, 0.05);
    glColor3f(1.0, 1.0, 1.0);
    bullet();
    glDeleteTextures(1, &textureArr[0]);
    glPopMatrix();
}

void shootingPistol() {
    glPushMatrix();
    pistol();
    glPopMatrix();

    glPushMatrix();
    bulletShoot();
    glPopMatrix();
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
    //gluQuadricTexture(cyObj, GL_TRUE);
    //gluQuadricNormals(cyObj, GLU_SMOOTH);

    // bind texture to object
    //glBindTexture(GL_TEXTURE_2D, innerbody);
    
    if (isGun) {
        glPushMatrix();
            glEnable(GL_TEXTURE_2D);
            glTranslatef(1.6f, 0.9f, 0.0f);
            glScalef(2.5f, 2.5f, 2.5f);
            shootingPistol();
            glDisable(GL_TEXTURE_2D);
        glPopMatrix();
    }
    
    // Inner left leg
    glPushMatrix();
        glTranslatef(-.3f, -.201f, .0f);
        glRotatef(leftLegRotationAngle, 1.0f, .0f, .0f); // TODO: Rotate the leg like walking and running animation
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
        glRotatef(rightLegRotationAngle, 1.0f, .0f, .0f);
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
    drawInnerChest();

    // outer chest
    drawOuterChest();

    
    // whole left arm
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

    glPushMatrix();
        glTranslatef(-.65f, .8f, .0f);
        drawLeftArm();
    glPopMatrix();
    

    glPushMatrix();
        glTranslatef(0.65f, .8f, .0f);
        drawRightArm();
    glPopMatrix();
    
    // Inner head
    drawInnerHead();

    glPushMatrix();
        glTranslatef(.0f, .29f, -.2f);
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


void updateAnimation(int value) {
    if (isWalking || isRunning) {
        // Adjust speed based on walking or running
        float speedMultiplier = isRunning ? 3.0f : 1.0f;

        // Update leg rotation
        if (legDirection) {
            leftLegRotationAngle += animationSpeed * speedMultiplier;
            rightLegRotationAngle -= animationSpeed * speedMultiplier;
        }
        else {
            leftLegRotationAngle -= animationSpeed * speedMultiplier;
            rightLegRotationAngle += animationSpeed * speedMultiplier;
        }

        if (leftLegRotationAngle >= 120.0f || leftLegRotationAngle <= 60.0f)
            legDirection = !legDirection;

        // Update arm rotation
        if (armDirection) {
            leftWholeArmRotateX -= animationSpeed * speedMultiplier;
            rightWholeArmRotateX += animationSpeed * speedMultiplier;
        }
        else {
            leftWholeArmRotateX += animationSpeed * speedMultiplier;
            rightWholeArmRotateX -= animationSpeed * speedMultiplier;
        }

        if (leftWholeArmRotateX >= 120.0f || leftWholeArmRotateX <= 60.0f)
            armDirection = !armDirection;

        // Redraw the scene
        glutPostRedisplay();
    }

    if (isShooting) {
        static bool pistolShotInitialized = false; // To ensure shootingPistol is called only once

        if (wholeArmRotateZ < 90.0f) {
            wholeArmRotateZ += 3.0f;
        }
        else {
            if (baseAngle < BASE_ANGLE_MAX) baseAngle += 1.0f;
            if (middleAngle < MIDDLE_ANGLE_MAX) middleAngle += 1.5f;
            if (tipAngle < TIP_ANGLE_MAX) tipAngle += 2.0f;
        }

        // Check if arm rotation and finger bending are complete
        if (wholeArmRotateZ >= 90.0f && baseAngle >= BASE_ANGLE_MAX &&
            middleAngle >= MIDDLE_ANGLE_MAX && tipAngle >= TIP_ANGLE_MAX) {
            if (!pistolShotInitialized) {
               
                isGun = true;
            }
            
            std::cout << "Shooting Pistol" << std::endl;
        }


        // Redraw the scene
        glutPostRedisplay();
    }

    // Call this function again after 16ms (approx. 60fps)
    glutTimerFunc(16, updateAnimation, 0);
}


void initAnimation() {
    // Start the animation update loop
    glutTimerFunc(16, updateAnimation, 0);
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

    initAnimation();
    glutMainLoop();                               // Enter main event loop

    return 0;
}
