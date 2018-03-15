#include "stdafx.h"
#include <GL/glut.h>

///////////////////////////////////////////////////////////
// Wywoływana w celu przerysowania sceny
void RenderScene(void) {
	// Wyczyszczenie okna aktualnym kolorem czyszczcym   
	glClear(GL_COLOR_BUFFER_BIT);
	// Aktualny kolor rysujcy - czerwony  
	//           R     G     B  
	glColor3f(0.2f, 0.0f, 0.5f);
	// Narysowanie prostokta wypełnionego aktualnym kolorem  
	glRectf(-10.0f, 10.0f, 10.0f, -10.0f);
	// Wysłanie polece do wykonania     
	glFlush();
}
///////////////////////////////////////////////////////////
// Konfiguracja stanu renderowania  
void SetupRC(void) {
	// Ustalenie niebieskiego koloru czyszczcego     
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f);
}
///////////////////////////////////////////////////////////
// Wywoływana przez bibliotek GLUT w przypadku zmiany rozmiaru okna
void ChangeSize(int w, int h) {
	GLfloat aspectRatio;
	// Zabezpieczenie przed dzieleniem przez zero  
	if (h == 0)   h = 1;
	// Ustawienie wielkości widoku na równą wielkości okna    
	glViewport(0, 0, w, h);
	// Ustalenie układu współrzędnych  
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Wyznaczenie przestrzeni ograniczającej (lewy, prawy, dolny, górny, bliski, odległy)  
	aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)    glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
	else    glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
	glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Kwadrat");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutMainLoop();
	return 0;
}