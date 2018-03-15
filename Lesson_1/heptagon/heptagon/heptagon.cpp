#include "stdafx.h"
#include <cmath> // biblioteka funckji matematycznej 
#include <GL/glut.h> // specyfikacja otwartego i uniwersalnego API do tworzenia grafiki

// Pocztkowy rozmiar i pozycja prostokta
GLfloat x_1 = 100.0f; //pozycja przesunięcia względem osi x 
GLfloat y_1 = 100.0f; //pozycja przesunięcia względem osi y
GLsizei rsize = 50; // granice krawędzi


					// Rozmiar kroku (liczba pikseli) w osi x i y - prędość
GLfloat xstep = 3.0f;
GLfloat ystep = 3.0f;

// Dane zmieniajcych się rozmiarów okna
GLfloat windowWidth;
GLfloat windowHeight;

//-------------------------------------------------------------------------------------------

// Wywoływana w celu przerysowania sceny

void RenderScene(void) {

	glClear(GL_COLOR_BUFFER_BIT); // Wyczyszczenie okna aktualnym kolorem czyszczącym
	glColor3f(1.000, 0.843, 0.000); // Aktualny kolor rysujący - złoty


									// Narysowanie prostokąta wypełnionego aktualnym kolorem
	int amount = 7; // określam ilość boków
	glBegin(GL_TRIANGLE_FAN); // Trójkąt.Każde trzy kolejne glVertex##() definiują wierzchołki trójkąta.
	glVertex2f(x_1, y_1);

	for (int i = 0; i <= amount; i++)
	{
		glVertex2f(
			(x_1 + (10.0 * sin(i * 2 * 3.1416f / amount)) * 5), //3.1416 określa zamknięcie i złożenie kształtu, 5- określa wielkość kształtu
			(y_1 + (10.0 * cos(i * 2 * 3.1416f / amount)) * 5) // sin i cos pozwala utworzyć kształt przypominający "koło"
		);
	}
	glEnd();
	glFlush();

	glutSwapBuffers(); // Wysłanie poleceń do wykonania - !!! dla animacji to jest inne polecenie
}

///////////////////////////////////////////////////////////

// Wywoływana przez bibliotek GLUT w czasie, gdy okno nie
// jest przesuwane ani nie jest zmieniana jego wielkoć

void TimerFunction(int value) {

	// Odwrócenie kierunku, jeżeli osięgnięto lewo lub prawo krawędzi
	if (x_1 > windowWidth - rsize || x_1 + (10.0 * cos(3 * 2 * 3.14f / 5)) * 5 < 0)
		xstep = -xstep;


	// Odwrócenie kierunku, jeżeli osięgnięto dolną lub górną krawędź
	if (y_1 > windowHeight - rsize || y_1 - rsize < 0)
		ystep = -ystep;


	/*
	Kontrola obramowania. Wykonywana jest na wypadek, gdyby okno
	zmniejszyło swoj wielkoć w czasie, gdy kwadrat odbijał się od
	krawędzi, co mogłoby spowodować, że znalazł by się poza
	przestrzenią ograniczajcą.
	*/

	if (x_1 > windowWidth - rsize)
		x_1 = windowWidth - rsize - 1;


	if (y_1 > windowHeight - rsize)
		y_1 = windowHeight - rsize - 1;



	// Wykonanie przesunięcia kwadratu
	x_1 += xstep;
	y_1 += ystep;



	// Ponowne rysowanie sceny z nowymi współrzędnymi
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

///////////////////////////////////////////////////////////

// Konfigurowanie stanu renderowania

void SetupRC(void) {

	// Ustalenie niebieskiego koloru czyszczącego
	glClearColor(0.600, 0.196, 0.800, 0.000);

}

///////////////////////////////////////////////////////////

// Wywoływana przez bibliotek GLUT przy każdej zmianie wielkoci okna

void ChangeSize(GLsizei w, GLsizei h) {
	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;



	// Ustalenie wielkoci widoku zgodnego z rozmiarami okna

	glViewport(0, 0, w, h);



	// Ustalenie układu współrzędnych

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	// Keep the square square, this time, save calculated

	// szerokość i wykość dla późniejszego użycia

	if (w <= h)
	{
		windowHeight = 250.0f*h / w;
		windowWidth = 250.0f;
	}
	else
	{
		windowWidth = 250.0f*w / h;
		windowHeight = 250.0f;
	}



	// Ustanowienie przestrzeni ograniczającej (lewo, prawo, dół, góra, blisko, daleko)
	glOrtho(0.0f, windowWidth, 0.0f, windowHeight, 1.0f, -1.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}

///////////////////////////////////////////////////////////

// Główny punkt wejcia programu

void main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Siedmiokąt");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(33, TimerFunction, 1);
	SetupRC();
	glutMainLoop();
}
