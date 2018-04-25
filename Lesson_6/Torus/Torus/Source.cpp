// opengl_przekstalcenia.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
/*
(c) Janusz Ganczarski
http://www.januszg.hg.pl
JanuszG@enter.net.pl
*/
#include <GL/glut.h>
#include <stdlib.h>
// stałe do obsługi menu podręcznego
enum
{
	FULL_WINDOW, // aspekt obrazu - całe okno
	ASPECT_1_1, // aspekt obrazu 1:1
	WIRE_SPHERE, // kula
	WIRE_CONE, // stożek
	WIRE_CUBE, // sześcian
	WIRE_TORUS, // torus
	WIRE_DODECAHEDRON, // dwunastościan
	WIRE_TEAPOT, // czajnik
	WIRE_OCTAHEDRON, // ośmiościan
	WIRE_TETRAHEDRON, // czworościan
	WIRE_ICOSAHEDRON, // dwudziestościan
	EXIT // wyjście
};
// aspekt obrazu
int aspect = FULL_WINDOW;
// rodzaj obiektu
int object = WIRE_SPHERE;
// rozmiary bryły obcinania
const GLdouble left = -10.0;
const GLdouble right = 10.0;
const GLdouble bottom = -10.0;
const GLdouble top = 10.0;
const GLdouble near_ = 50.0;
const GLdouble far_ = 70.0;
// współczynnik skalowania
GLfloat scale = 1.0;
// kąty obrotu
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;
// przesunięcie
GLfloat translatex = 0.0;
GLfloat translatey = 0.0;
// wskaŸnik naciśnięcia lewego przycisku myszki
int button_state = GLUT_UP;
// położenie kursora myszki
int button_x, button_y;
// funkcja generująca scenę 3D
void Display()
{
	// kolor tła - zawartość bufora koloru
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// czyszczenie bufora koloru
	glClear(GL_COLOR_BUFFER_BIT);
	// wybór macierzy modelowania
	glMatrixMode(GL_MODELVIEW);
	// macierz modelowania = macierz jednostkowa
	glLoadIdentity();
	// przesunięcie układu współrzędnych obiektu do środka bryły odcinania
	glTranslatef(0, 0, -(near_ + far_) / 2);
	// przesunięcie obiektu - ruch myszką
	glTranslatef(translatex, translatey, 0.0);
	// skalowanie obiektu - klawisze "+" i "-"
	glScalef(scale, scale, scale);
	// obroty obiektu - klawisze kursora
	glRotatef(rotatex, 1.0, 0, 0);
	glRotatef(rotatey, 0, 1.0, 0);
	// kolor krawędzi obiektu
	glColor3f(0.0, 0.0, 0.0);
	// rysowanie obiektu
	switch (object)
	{
		// kula
	case WIRE_SPHERE:
		glutWireSphere(1.0, 20, 10);
		break;
		// stożek
	case WIRE_CONE:
		glutWireCone(1.0, 2.0, 20, 10);
		break;
		// sześcian
	case WIRE_CUBE:
		glutWireCube(1.0);
		break;
		// torus
	case WIRE_TORUS:
		glutWireTorus(0.2, 1, 10, 20);
		break;
		// dwunastościan
	case WIRE_DODECAHEDRON:
		glutWireDodecahedron();
		break;
		// czajnik
	case WIRE_TEAPOT:
		glutWireTeapot(1.0);
		break;
		// ośmiościan
	case WIRE_OCTAHEDRON:
		glutWireOctahedron();
		break;
		// czworościan
	case WIRE_TETRAHEDRON:
		glutWireTetrahedron();
		break;
		// dwudziestościan
	case WIRE_ICOSAHEDRON:
		glutWireIcosahedron();
		break;
	}
	// skierowanie poleceń do wykonania
	glFlush();
	// zamiana buforów koloru
	glutSwapBuffers();
}
// zmiana wielkości okna
void Reshape(int width, int height)
{
	// obszar renderingu - całe okno
	glViewport(0, 0, width, height);
	// wybór macierzy rzutowania
	glMatrixMode(GL_PROJECTION);
	// macierz rzutowania = macierz jednostkowa
	glLoadIdentity();
	// parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		// wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom* height / width, top* height / width, near_,
				far_);
		else
			// szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left* width / height, right* width / height, bottom, top,
					near_, far_);
	}
	else
		glFrustum(left, right, bottom, top, near_, far_);
	// generowanie sceny 3D
	Display();
}
// obsługa klawiatury
void Keyboard(unsigned char key, int x, int y)
{
	// klawisz +
	if (key == '+')
		scale += 0.1;
	else
		// klawisz -
		if (key == '-' && scale > 0.1)
			scale -= 0.1;
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// obsługa klawiszy funkcyjnych i klawiszy kursora
void SpecialKeys(int key, int x, int y)
{
	switch (key)
	{
		// kursor w lewo
	case GLUT_KEY_LEFT:
		rotatey -= 1;
		break;
		// kursor w górę
	case GLUT_KEY_UP:
		rotatex -= 1;
		break;
		// kursor w prawo
	case GLUT_KEY_RIGHT:
		rotatey += 1;
		break;
		// kursor w dół
	case GLUT_KEY_DOWN:
		rotatex += 1;
		break;
	}
	// odrysowanie okna
	Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}
// obsługa przycisków myszki
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		// zapamiętanie stanu lewego przycisku myszki
		button_state = state;
		// zapamiętanie położenia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}
// obsługa ruchu kursora myszki
void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		translatex += 1.1 *(right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		translatey += 1.1 *(top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}
// obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
		// obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
		// obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;
		// kula
	case WIRE_SPHERE:
		object = WIRE_SPHERE;
		Display();
		break;
		// cylinder
	case WIRE_CONE:
		object = WIRE_CONE;
		Display();
		break;
		// sześcian
	case WIRE_CUBE:
		object = WIRE_CUBE;
		Display();
		break;
		// torus
	case WIRE_TORUS:
		object = WIRE_TORUS;
		Display();
		break;
		// dwunastościan
	case WIRE_DODECAHEDRON:
		object = WIRE_DODECAHEDRON;
		Display();
		break;
		// czajnik
	case WIRE_TEAPOT:
		object = WIRE_TEAPOT;
		Display();
		break;
		// ośmiościan
	case WIRE_OCTAHEDRON:
		object = WIRE_OCTAHEDRON;
		Display();
		break;
		// czworościan
	case WIRE_TETRAHEDRON:
		object = WIRE_TETRAHEDRON;
		Display();
		break;
		// dwudziestościan
	case WIRE_ICOSAHEDRON:
		object = WIRE_ICOSAHEDRON;
		Display();
		break;
		// wyjście
	case EXIT:
		exit(0);
	}
}