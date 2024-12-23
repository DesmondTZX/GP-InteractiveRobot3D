#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800

// Function to handle key presses
void handleKeys(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		std::cout << "Exiting..." << std::endl;
		exit(0);
	default:
		break;
	}
	glutPostRedisplay();
}

void handleSpecialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        break;
    case GLUT_KEY_DOWN:
        break;
    case GLUT_KEY_RIGHT:
        break;
    case GLUT_KEY_LEFT:
        break;
    default:
        break;
    }
    glutPostRedisplay(); // Request to update the display
}

// Display callback function
void display() {
	// set bg color
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glFlush();                            // Render the OpenGL commands
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);                                           // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);        // Set display mode
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                 // Set window size
    glutCreateWindow("BACS2173 - Practical 8 (Exercise)");          // Create window with title

	glEnable(GL_DEPTH_TEST); // Enable depth testing

    glutDisplayFunc(display);

    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, .1, 100.0);

    glutKeyboardFunc(handleKeys);
    glutSpecialFunc(handleSpecialKeys);
    glutMainLoop();                               // Enter main event loop

    return 0;
}
