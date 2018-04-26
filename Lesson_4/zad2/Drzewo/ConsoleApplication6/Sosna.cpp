#include <GL/glut.h>
#include "glext.h"
#ifndef WIN32
#define GLX_GLXEXT_LEGACY
#include <GL/glx.h>
#define wglGetProcAddress glXGetProcAddressARB
#endif
#include <stdlib.h>
#include <stdio.h>
#include "colors.h"
#include "targa.h"

PFNGLWINDOWPOS2IPROC glWindowPos2i = NULL; //wskaźnik na funkcję glWindowPos2i

//stałe do obsługi menu podręcznego
enum
{
	FULL_WINDOW,    //aspekt obrazu - całe okno
	ASPECT_1_1,     //aspekt obrazu 1:1
	EXIT            //wyjście
};

int aspect = FULL_WINDOW; //aspekt obrazu

//usunięcie definicji makr near i far
#ifdef near
#undef near
#endif
#ifdef far
#undef far
#endif

//rozmiary bryły obcinania
const GLdouble left = -2.0;
const GLdouble right = 2.0;
const GLdouble bottom = -2.0;
const GLdouble top = 2.0;
const GLdouble near = 3.0;
const GLdouble far = 7.0;

//kąty obrotu
GLfloat rotatex = 0.0;
GLfloat rotatey = 0.0;

int button_state = GLUT_UP; //wskaźnik naciśnięcia lewego przycisku myszki
int button_x, button_y; //położenie kursora myszki

GLfloat scale = 1.5; //współczynnik skalowania
GLuint GRASS, TREE; //identyfikatory tekstur
GLint GRASS_LIST, TREE_LIST; //identyfikatory list wyświetlania

//włączenie/wyłączenie
bool alpha_test = true;
bool blend = false;

//funkcja rysująca napis w wybranym miejscu (wersja korzystająca z funkcji glWindowPos2i)
void DrawString(GLint x, GLint y, char *string)
{
	glWindowPos2i(x, y); //położenie napisu

	//wyświetlenie napisu
	int len = strlen(string);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, string[i]);
}

//funkcja generująca scenę 3D
void DisplayScene()
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //kolor tła - zawartośæ bufora koloru
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //czyszczenie bufora koloru i bufora głębokości
	glMatrixMode(GL_MODELVIEW); //wybór macierzy modelowania
	glLoadIdentity(); //macierz modelowania = macierz jednostkowa
	glTranslatef(0.0, 0.0, -(near + far) / 2); //przesunięcie układu współrzędnych obiektów do środka bryły odcinania

	//obroty obiektu
	glRotatef(rotatex, 1.0, 0.0, 0.0);
	glRotatef(rotatey, 0.0, 1.0, 0.0);

	glScalef(scale, scale, scale); //skalowanie obiektu - klawisze "+" i "-"
	glEnable(GL_DEPTH_TEST); //włączenie testu bufora głębokości
	glEnable(GL_TEXTURE_2D); //włączenie teksturowania dwuwymiarowego
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //filtr powiększający
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //filtr pomniejszający
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); //wskazówki do korekcji perspektywy przy renderingu tekstur
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); //ustawienie parametów środowiska tekstur

	//włączenie/wyłączenie testowania kanału alfa
	if (alpha_test == GL_TRUE)
	{
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5);
	}
	else
		glDisable(GL_ALPHA_TEST);

	//włączenie/wyłączenie mieszania kolorów
	if (blend == GL_TRUE)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
		glDisable(GL_BLEND);

	//rysowanie trawy
	glBindTexture(GL_TEXTURE_2D, GRASS);
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0);
	glCallList(GRASS_LIST);
	glPopMatrix();

	//rysowanie trzech drzew
	glBindTexture(GL_TEXTURE_2D, TREE);
	glPushMatrix();
	glTranslatef(-0.5, 0.0, 0.0);
	glScalef(0.5, 0.5, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glTranslatef(1.0, 0.0, 0.5);
	glCallList(TREE_LIST);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D); //wyłączenie teksturowania dwuwymiarowego

	//wyświetlenie wybranych informacje
	char string[200];
	GLboolean var;
	glColor3fv(Black);

	//pobranie inforacji o włączeniu testowania kanału alfa
	glGetBooleanv(GL_ALPHA_TEST, &var);
	if (var == GL_TRUE)
		strcpy(string, "GL_ALPHA_TEST = GL_TRUE");
	else
		strcpy(string, "GL_ALPHA_TEST = GL_FALSE");
	DrawString(2, glutGet(GLUT_WINDOW_HEIGHT) - 17, string);

	//pobranie informacji o włączeniu mieszania kolorów
	glGetBooleanv(GL_BLEND, &var);
	if (var == GL_TRUE)
		strcpy(string, "GL_BLEND = GL_TRUE");
	else
		strcpy(string, "GL_BLEND = GL_FALSE");
	DrawString(2, glutGet(GLUT_WINDOW_HEIGHT) - 33, string);

	glFlush(); //skierowanie poleceń do wykonania
	glutSwapBuffers(); //zamiana buforów koloru
}

//zmiana wielkości okna
void Reshape(int width, int height)
{
	glViewport(0, 0, width, height); //obszar renderingu - całe okno
	glMatrixMode(GL_PROJECTION); //wybór macierzy rzutowania
	glLoadIdentity(); //macierz rzutowania = macierz jednostkowa

	//parametry bryły obcinania
	if (aspect == ASPECT_1_1)
	{
		//wysokość okna większa od wysokości okna
		if (width < height && width > 0)
			glFrustum(left, right, bottom*height / width, top*height / width, near, far);
		else
			//szerokość okna większa lub równa wysokości okna
			if (width >= height && height > 0)
				glFrustum(left*width / height, right*width / height, bottom, top, near, far);
	}
	else
		glFrustum(left, right, bottom, top, near, far);

	DisplayScene(); //generowanie sceny 3D
}

//obsługa klawiatury
void Keyboard(unsigned char key, int x, int y)
{
	//klawisz +
	if (key == '+')
		scale += 0.05;
	else
		//klawisz -
		if (key == '-' && scale > 0.05)
			scale -= 0.05;
	
	DisplayScene(); //narysowanie sceny
}

//obsługa przycisków myszki
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		//zapamiętanie stanu lewego przycisku myszki
		button_state = state;

		//zapamiętanie położenia kursora myszki
		if (state == GLUT_DOWN)
		{
			button_x = x;
			button_y = y;
		}
	}
}

//obsługa ruchu kursora myszki
void MouseMotion(int x, int y)
{
	if (button_state == GLUT_DOWN)
	{
		rotatey += 30 * (right - left) / glutGet(GLUT_WINDOW_WIDTH) * (x - button_x);
		button_x = x;
		rotatex -= 30 * (top - bottom) / glutGet(GLUT_WINDOW_HEIGHT) * (button_y - y);
		button_y = y;
		glutPostRedisplay();
	}
}

//utworzenie tekstur
void GenerateTextures()
{
	//zmienne użyte przy obsłudze plików TARGA
	GLsizei width, height;
	GLenum format, type;
	GLvoid *pixels;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //tryb upakowania bajtów danych tekstury
	GLboolean error = load_targa("grass_color.tga", width, height, format, type, pixels); //wczytanie tekstury grass_color.tga

	//błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku grass_color.tga");
		exit(0);
	}

	glGenTextures(1, &GRASS); //utworzenie identyfikatora tekstury
	glBindTexture(GL_TEXTURE_2D, GRASS); //dowiązanie stanu tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); //włączenie automatycznego generowania mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, type, pixels); //definiowanie tekstury (z mipmapami)
	delete[](unsigned char*)pixels; //porządki
	error = load_targa("pine_tree.tga", width, height, format, type, pixels); //wczytanie tekstury pine_tree.tga

	//błąd odczytu pliku
	if (error == GL_FALSE)
	{
		printf("Niepoprawny odczyt pliku pine_tree.tga");
		exit(0);
	}

	
	glGenTextures(1, &TREE); //utworzenie identyfikatora tekstury
	glBindTexture(GL_TEXTURE_2D, TREE); //dowiązanie stanu tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); //włączenie automatycznego generowania mipmap
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, type, pixels); //definiowanie tekstury (z mipmapami)
	delete[](unsigned char*)pixels; //porządki
}

//obsługa menu podręcznego
void Menu(int value)
{
	switch (value)
	{
		//test kanału alfa: włącz/wyłącz
	case GL_ALPHA_TEST:
		alpha_test = !alpha_test;
		DisplayScene();
		break;

		//mieszanie kolorów: włącz/wyłącz
	case GL_BLEND:
		blend = !blend;
		DisplayScene();
		break;

		//obszar renderingu - całe okno
	case FULL_WINDOW:
		aspect = FULL_WINDOW;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		//obszar renderingu - aspekt 1:1
	case ASPECT_1_1:
		aspect = ASPECT_1_1;
		Reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		break;

		//wyjście
	case EXIT:
		exit(0);
	}
}

//utworzenie list wyświetlania
void GenerateDisplayLists()
{
	GRASS_LIST = glGenLists(1); //generowanie identyfikatora pierwszej listy wyświetlania
	glNewList(GRASS_LIST, GL_COMPILE); //pierwsza lista wyświetlania - trawa

	//czworokąt
	glBegin(GL_QUADS);
	glTexCoord2f(16.0, 0.0);
	glVertex3f(-8.0, 0.0, -8.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-8.0, 0.0, 8.0);
	glTexCoord2f(0.0, 16.0);
	glVertex3f(8.0, 0.0, 8.0);
	glTexCoord2f(16.0, 16.0);
	glVertex3f(8.0, 0.0, -8.0);
	glEnd();

	
	glEndList(); //koniec pierwszej listy wyświetlania
	TREE_LIST = glGenLists(1); //generowanie identyfikatora drugiej listy wyświetlania
	glNewList(TREE_LIST, GL_COMPILE); //druga lista wyświetlania - drzewo

	//dwa trójkąty
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 0.0);
	glVertex3f(1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 0.0);
	glVertex3f(-1.0, -1.0, 0.0);
	glTexCoord2f(1.0, 1.0);
	glVertex3f(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0);
	glVertex3f(-1.0, 1.0, 0.0);
	glEnd();

	glEndList(); //koniec drugiej listy wyświetlania
}

//sprawdzenie i przygotowanie obsługi wybranych rozszerzeń
void ExtensionSetup()
{
	const char *version = (char*)glGetString(GL_VERSION); //pobranie numeru wersji biblioteki OpenGL

	//odczyt wersji OpenGL
	int major = 0, minor = 0;
	if (sscanf(version, "%d.%d", &major, &minor) != 2)
	{
		#ifdef WIN32
			printf("Błędny format wersji OpenGL\n");
		#else

			printf("Bledny format wersji OpenGL\n");
		#endif
		exit(0);
	}

	//sprawdzenie czy jest co najmniej wersja 1.4 OpenGL lub czy jest obsługiwane rozszerzenie GL_SGIS_generate_mipmap
	if (!(major > 1 || minor >= 4) &&
		!glutExtensionSupported("GL_SGIS_generate_mipmap"))
	{
		printf("Brak rozszerzenia GL_SGIS_generate_mipmap!\n");
		exit(0);
	}

	//sprawdzenie czy jest co najmniej wersja 1.4
	if (major > 1 || minor >= 4)
	{
		glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress("glWindowPos2i"); //pobranie wskaźnika wybranej funkcji OpenGL 1.4
	}
	else
		//sprawdzenie czy jest obsługiwane rozszerzenie ARB_window_pos
		if (glutExtensionSupported("GL_ARB_window_pos"))
		{
			//pobranie wskaźnika wybranej funkcji rozszerzenia ARB_window_pos
			glWindowPos2i = (PFNGLWINDOWPOS2IPROC)wglGetProcAddress
				("glWindowPos2iARB");
		}
		else
		{
			printf("Brak rozszerzenia ARB_window_pos!\n");
			exit(0);
		}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv); //inicjalizacja biblioteki GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); //inicjalizacja bufora ramki
	glutInitWindowSize(500, 500); //rozmiary głównego okna programu
	glutCreateWindow("Tekstura alfa"); //utworzenie głównego okna programu
	glutDisplayFunc(DisplayScene); //dołączenie funkcji generującej scenę 3D
	glutReshapeFunc(Reshape); //dołączenie funkcji wywoływanej przy zmianie rozmiaru okna
	glutKeyboardFunc(Keyboard); //dołączenie funkcji obsługi klawiatury
	glutMouseFunc(MouseButton); //obsługa przycisków myszki
	glutMotionFunc(MouseMotion); //obsługa ruchu kursora myszki
	int MenuAspect = glutCreateMenu(Menu); //utworzenie podmenu - Aspekt obrazu
	#ifdef WIN32
		glutAddMenuEntry("Aspekt obrazu - całe okno", FULL_WINDOW);
	#else
		glutAddMenuEntry("Aspekt obrazu - cale okno", FULL_WINDOW);
	#endif
	glutAddMenuEntry("Aspekt obrazu 1:1", ASPECT_1_1);
	glutCreateMenu(Menu); //menu główne
	#ifdef WIN32
		glutAddMenuEntry("Test kanału alfa: włącz/wyłącz", GL_ALPHA_TEST);
		glutAddMenuEntry("Mieszanie kolorów: włącz/wyłącz", GL_BLEND);
		glutAddSubMenu("Aspekt obrazu", MenuAspect);
		glutAddMenuEntry("Wyjście", EXIT);
	#else
		glutAddMenuEntry("Test kanalu alfa: wlacz/wylacz", GL_ALPHA_TEST);
		glutAddMenuEntry("Mieszanie kolorow: wlacz/wylacz", GL_BLEND);
		glutAddSubMenu("Aspekt obrazu", MenuAspect);
		glutAddMenuEntry("Wyjscie", EXIT);
	#endif
	glutAttachMenu(GLUT_RIGHT_BUTTON); //określenie przycisku myszki obsługującego menu podręczne
	GenerateTextures(); //utworzenie tekstur
	ExtensionSetup(); //sprawdzenie i przygotowanie obsługi wybranych rozszerzeń
	GenerateDisplayLists(); //utworzenie list wyświetlania
	glutMainLoop(); //wprowadzenie programu do obsługi pętli komunikatów
	return 0;
}
