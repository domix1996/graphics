#include "stdafx.h" //biblioteka pozwalajaca przyspieszenie kompilacji programu C
#include "math.h" //biblioteka funkcji matematycznej
//Jeśli zdefiniowane na komputer APPLE
//{
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h> //specyfikacja otwartego i uniwersalnego API do tworzenia grafiki
#endif
//}

#define GL_PI 3.1415f //zdefiniowany typ PI

enum
{
	EXIT // wyjście - wybór z listy programu
};

// Wielkość obrotów
static GLfloat xRot = 0.0f; //początkowy obrót względem osi x
static GLfloat yRot = 0.0f; //początkowy obrót względem osi y

static GLboolean iCull, iDepth, iOutline; //Zmienne Glboolean używane są do przechowywania wartości logicznych,

//Funkcja wykonuje wszystkie konieczne inicjalizacje kontekstu renderowania
void SetupRC()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //Czarne tło
	glColor3f(0.0f, 1.0f, 0.0f); //Kolor rysujący - zielony
	glShadeModel(GL_FLAT); //Model cieniowania kolorów - płaski
	glFrontFace(GL_CW); //Wielokąty o nawinięciu zgodnym z ruchem wskazówek zegara traktowane są jako skierowane do przodu. Takie ustawienie jest konieczne, ponieważkorzystamy z wachlarzy trójkątów.
}

//Wywoływana w celu przerysowania sceny
void RenderScene(void)
{
	GLfloat x, y, angle; //Przechowują wartości współrzędnych i kąta
	int iPivot = 1; //Do oznaczania zamiennych kolorów
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyszczenie okna i bufora głębi

	// Włączenie lub wyłączenie mechanizmu eliminowania ukrytych powierzchni
	if (iCull)
		glEnable(GL_CULL_FACE); //określić, czy można wybrać ujęcie z przodu lub z tyłu w stosunku do wielokątów
	else
		glDisable(GL_CULL_FACE); //-.-.-

	// Włączenie lub wyłączenie mechanizmu sprawdzania głębi
	if (iDepth)
		glEnable(GL_DEPTH_TEST); //Jeśli jest włączona, wykonaj dokładne porównania i zaktualizuj bufor głębi.Zwróć uwagę, że nawet jeśli bufor głębi istnieje, a maska ​​głębi jest niezerowa, bufor głębi nie zostanie zaktualizowany, jeśli test głębokości zostanie wyłączony
	else
		glDisable(GL_DEPTH_TEST); //-.-.-

	// Jeżeli ten znacznik będzie ustawiony, to wielokąty zwrócone tyłem do widza będą rysowane tylko w postaci szkieletu
	if (iOutline)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);

	//Zapisanie stanu macierzy i wykonanie obrotu
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLE_FAN); //Rozpoczęcie rysowania wachlarza trójkątów
	glVertex3f(0.0f, 0.0f, 75.0f); //Czubek stożka jest wspólnym wierzchołkiem wszystkich trójkątów z wachlarza wysuniętym do góry w osi z. W ten sposób zamiast koła powstanie stożek.

	//Wykonujemy obrót w około i oznaczamy w równych odstępach wierzchołki tworzące wachlarz trójkątów.
	for (angle = 0.0f; angle < (4.5f*GL_PI); angle += (GL_PI / 4.0f))
	{
		//Wyliczenie współrzędnych x i y kolejnego wierzchołka
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		//Wybieranie koloru - zielony lub czerwony
		if ((iPivot % 2) == 0)
			glColor3f(1.0f, 0.9f, 0.6f);
		else
			glColor3f(0.5f, 0.1f, 0.1f);
		
		iPivot++; //Inkrementacja zmiennej okrelającej rodzaj koloru
		glVertex2f(x, y); //Definiowanie kolejnego wierzchołka w wachlarzu trójkątów
	}

	glEnd(); //Zakoczenie rysowania trójkątów stożka
	glBegin(GL_TRIANGLE_FAN); //Rozpoczęcie rysowania kolejnego wachlarza trójkątów zakrywającego podstaw stoka
	glVertex2f(0.0f, 0.0f); //środek wachlarza znajduje się na początku układu współrzędnych

	for (angle = 0.0f; angle < (4.0f*GL_PI); angle += (GL_PI / 4.f))
	{
		//Wyliczenie współrzędnych x i y kolejnego wierzchołka
		x = 50.0f*sin(angle);
		y = 50.0f*cos(angle);

		//Wybieranie koloru - zielony lub czerwony
		if ((iPivot % 2) == 0)
			glColor3f(1.0f, 0.9f, 0.6f);
		else
			glColor3f(0.5f, 0.1f, 0.1f);
		
		iPivot++; //Inkrementacja zmiennej okrelającej rodzaj koloru
		glVertex2f(x, y); //Definiowanie kolejnego wierzchołka w wachlarzu trójkątów
	}

	glEnd(); //Zakoczenie rysowania trójkątów podstawy stożka
	glPopMatrix(); //Odtworzenie macierzy przekształceń
	glutSwapBuffers(); //Wysłanie poleceń do wykonania
}

//sterowanie klawiszami
void SpecialKeys(int key, int x, int y)
{
	if (key == GLUT_KEY_UP)
		xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)
		xRot += 5.0f;
	if (key == GLUT_KEY_LEFT)
		yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT)
		yRot += 5.0f;
	if (key > 356.0f)
		xRot = 0.0f;
	if (key < -1.0f)
		xRot = 355.0f;
	if (key > 356.0f)
		yRot = 0.0f;
	if (key < -1.0f)
		yRot = 355.0f;

	glutPostRedisplay(); //Odświeżenie zawartości okna
}

//zmiana rozmiaru ekranu programu
void ChangeSize(int w, int h)
{
	GLfloat nRange = 100.0f; //skalowanie okna

	// Zabezpieczenie przed dzieleniem przez zero
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h); //Ustalenie wymiarów widoku na zgodnych z wymiarami okna
	glMatrixMode(GL_PROJECTION); //Ponowne ustawienie stosu macierzy rzutowania
	glLoadIdentity(); //zastąp bieżącą macierz macierzą tożsamości

	//Utworzenie przestrzeni ograniczającej (lewo, prawo, dół, góra, blisko, daleko)
	if (w <= h)
		glOrtho(-nRange, nRange, -nRange*h / w, nRange*h / w, -nRange, nRange);
	else
		glOrtho(-nRange*w / h, nRange*w / h, -nRange, nRange, -nRange, nRange);
	
	glMatrixMode(GL_MODELVIEW); //Ponowne ustawienie stosu macierzy rzutowania
	glLoadIdentity(); //zastąp bieżącą macierz macierzą tożsamości
}

void Menu(int value)
{
	switch (value)
	{
	case GL_CULL_FACE:
		iCull = !iCull;
		glutSetWindowTitle("Stozek - GL_CULL_FACE");
		RenderScene();
		break;

	case GL_DEPTH_TEST:
		iDepth = !iDepth;
		glutSetWindowTitle("Stozek - GL_DEPTH_TEST");
		RenderScene();
		break;

	case GL_LINE:
		iOutline = true;
		glutSetWindowTitle("Stozek - GL_LINE");
		RenderScene();
		break;

	case GL_FILL:
		iOutline = false;
		glutSetWindowTitle("Stozek - GL_LINE");
		RenderScene();
		break;

	case EXIT:
		exit(0);
	}
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Triangles Constructing Fan Example");
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	SetupRC();
	// utworzenie podmenu - Prymitywy
	int MenuPrimitive = glutCreateMenu(Menu);
	glutAddMenuEntry("GL_CULL_FACE", GL_CULL_FACE);
	glutAddMenuEntry("GL_DEPTH_TEST", GL_DEPTH_TEST);
	glutAddMenuEntry("Polygon mode LINE", GL_LINE);
	glutAddMenuEntry("Polygon mode FILL", GL_FILL);
	glutAddMenuEntry("Wyjście", EXIT);

	// określenie przycisku myszki obsługującego menu podręczne
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
}
