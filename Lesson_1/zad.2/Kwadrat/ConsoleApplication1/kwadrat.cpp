#include "stdafx.h" //biblioteka pozwalajaca przyspieszenie kompilacji programu C
#include <GL/glut.h> // nagłówek biblioteki OPENGL 

// Wywoływana w celu przerysowania sceny
void RenderScene(void) 
{   
	glClear(GL_COLOR_BUFFER_BIT); // Wyczyszczenie okna aktualnym kolorem czyszczącym
	glColor3f(0.2f, 0.0f, 0.5f); // (R,G,B) Aktualny kolor rysujący - czerwony  
	glRectf(-10.0f, 10.0f, 10.0f, -10.0f); // Narysowanie prostokta wypełnionego aktualnym kolorem     
	glFlush(); // Wysłanie polecenia do wykonania  
}

// Konfiguracja stanu renderowania  
void SetupRC(void) 
{    
	glClearColor(0.0f, 1.0f, 1.0f, 0.0f); // Ustalenie niebieskiego koloru czyszczącego 
}

// Wywoływana przez bibliotek GLUT w przypadku zmiany rozmiaru okna
void ChangeSize(int w, int h) 
{
	GLfloat aspectRatio;
	
	if (h == 0)   h = 1; // Zabezpieczenie przed dzieleniem przez zero  
	glViewport(0, 0, w, h); // Ustawienie wielkości widoku na równą wielkości okna    
	glMatrixMode(GL_PROJECTION); // Ustalenie układu współrzędnych  
	glLoadIdentity(); // funkcja zastępuje bieżącą macierz macierzą tożsamości.

	// Wyznaczenie przestrzeni ograniczającej (lewy, prawy, dolny, górny, bliski, odległy)
	//{
		aspectRatio = (GLfloat)w / (GLfloat)h;
		if (w <= h)    glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
		else    glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
		glMatrixMode(GL_MODELVIEW);  glLoadIdentity();
	//}
}

int main(int argc, char* argv[]) 
{
	glutInit(&argc, argv); //glutInit służy do inicjalizacji biblioteki GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 600); //ustalanie rozmiaru okna programu
	glutCreateWindow("Kwadrat"); //tworzenie okna z ustalaniem tytułu programu
	glutDisplayFunc(RenderScene); //glutDisplayFunc ustawia wywołanie zwrotne dla bieżącego okna .
	glutReshapeFunc(ChangeSize); //glutReshapeFunc ustawia wywołanie zwrotne dla zmiany rozmiaru okna .
	SetupRC(); //wywołanie funkcji ustalania stanu renderowania
	glutMainLoop(); //odtwarzanie programu w pętli aż do wymuszenia zakończenia działania przez użytkownika.
	return 0;
}