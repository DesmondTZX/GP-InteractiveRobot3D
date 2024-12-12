#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

#define _USE_MATH_DEFINES

#include <math.h>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800


void init() {
	glMatrixMode(GL_PROJECTION);
	// gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);    // Set orthogonal view for 2D
}

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

// Display callback function
void display() {
	// set bg color
	glClearColor(.0f, .0f, .0f, .0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glFlush();                            // Render the OpenGL commands
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);                                           // Initialize GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);                     // Set display mode
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);                 // Set window size
	glutCreateWindow("BACS2173 - Assignment");           // Create window with title

	init();                                       // Call initialization function

	glEnable(GL_DEPTH_TEST);                      // depth test after initialization

	glutDisplayFunc(display);                     // Set display callback function
	glutKeyboardFunc(handleKeys);
	glutMainLoop();                               // Enter main event loop

	return 0;
}
