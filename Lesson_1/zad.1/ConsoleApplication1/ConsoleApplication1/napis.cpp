// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <GL/glut.h>

void RenderScrene(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void SetupRC(void) {
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
}

void main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Mój pierwszy program w GLUT");
	glutDisplayFunc(RenderScrene);
	SetupRC();
	glutMainLoop();
}

