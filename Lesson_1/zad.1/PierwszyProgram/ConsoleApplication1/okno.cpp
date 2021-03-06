#include "stdafx.h" // biblioteka pozwalajaca przyspieszenie kompilacji programu C
#include <GL/glut.h> // nagłówek biblioteki OPENGL 

// Funkcja wywoływana w celu narysowania sceny
void RenderScrene(void) 
{
	glClear(GL_COLOR_BUFFER_BIT); // Wyczyszczenie okna aktualnym kolorem czyszczącym
	glFlush(); /// Przekazanie polecenia do wykonania
}

// Ustalenie stanu rendrowania
void SetupRC(void) 
{
	glClearColor(0.1f, 0.0f, 0.0f, 1.0f); //(R,G,B) ustalanie aktualnego koloru tła czyli czarn0-brązowy
}

void main(int argc, char **argv) 
{
	glutInit(&argc, argv); //glutInit służy do inicjalizacji biblioteki GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA); //ustawia początkowy tryb wyświetlania (Maska bitowa do wybrania pojedynczego buforowanego okna. Jest to ustawienie domyślne, jeśli nie podano GLUT_DOUBLE lub GLUT_SINGLE .| Maska bitowa do wybrania okna trybu RGBA. Jest to ustawienie domyślne, jeśli nie podano GLUT_RGBA ani GLUT_INDEX .)
	glutCreateWindow("Mój pierwszy program w GLUT"); //tworzenie okna z ustalaniem tytułu programu
	glutDisplayFunc(RenderScrene); // glutDisplayFunc ustawia wywołanie zwrotne dla bieżącego okna .
	SetupRC(); // wywołanie funkcji ustalania stanu renderowania
	glutMainLoop(); //odtwarzanie programu w pętli aż do wymuszenia zakończenia działania przez użytkownika
}

