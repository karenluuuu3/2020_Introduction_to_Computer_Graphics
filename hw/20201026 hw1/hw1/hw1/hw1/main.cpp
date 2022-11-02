#include "Object.h"
#include "glut.h"
#include <math.h>

int windowSize[2];

void light();
void display();
void reshape(GLsizei, GLsizei);
void LoadModel(Object*);
void DrawBasis();
void keyboard(unsigned char key, int x, int y);

Object* elephant = new Object("elephant.obj");
Object* cube = new Object("cube.obj");
Object* teapot = new Object("teapot.obj");


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	/// light();
	glutCreateWindow("0716207 §f«ä¨ç HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}

void light()
{
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_position[] = { 50.0, 75.0, 80.0, 1.0 };
	glShadeModel(GL_SMOOTH);
	// z buffer enable
	glEnable(GL_DEPTH_TEST);
	// enable lighting
	glEnable(GL_LIGHTING);
	// set light property
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
}

void display()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///TO DO Enable DepthTest
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LEQUAL);

	///TO DO: add lighting, Viewport Matrix, Projection Matrix, ModelView Matrix
	/// Viewport Matrix
	glViewport(0, 0, 500, 500);
	/// Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)windowSize[0] / (float)windowSize[1], 1.0, 1000.0);/// fovy, aspect, near, far
	/// ModelView Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);/// eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz

	light();

	///TO DO: add model, translate, rotate, scale
	DrawBasis();
	/// glTranslatef(0.0, 0.0, 0.0);

	glutPostRedisplay();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadModel(Object* Model)
{
	for (size_t i = 0; i < Model->fNum; ++i)
	{
		glBegin(GL_TRIANGLES);
		for (size_t j = 0; j < 3; ++j)
		{
			glNormal3fv(Model->nList[Model->faceList[i][j].nIndex].ptr);
			glVertex3fv(Model->vList[Model->faceList[i][j].vIndex].ptr);
		}
		glEnd();
	}
}

void DrawBasis()
{
	///TO DO: draw basis
	const double PI = 3.14159265358979323846;
	double r = 10;
	int sides = 20;

	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++) {
		double angle = PI * i * 2 / sides;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3d( r * cos(angle), 0.0, r * sin(angle));
	}
	glEnd();

	/// «Ø20¶ô°¼­±
}

void keyboard(unsigned char key, int x, int y)
{
	/// TO DO: Set the keyboard control
	
}