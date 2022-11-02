#include "Object.h"
#include "glut.h"
#include <math.h>

int windowSize[2];
float ang = 0.0, ang3 = 0.0;/// used for rotation

/// elements for keyboard control 
float translate_x = 0.0;
float translate_y = 0.0;
float translate_z = 0.0;
GLfloat  xAngle = 0.0, yAngle = 90.0, zAngle = 0.0;

void light();
void display();
void reshape(GLsizei, GLsizei);
void LoadModel(Object*);
void DrawBasis();
void keyboard(unsigned char key, int x, int y);
void timer(int);/// set timer func
void timer3(int);/// set timer func : 三倍速
void reset();/// keyboard control

Object* elephant = new Object("elephant.obj");
Object* cube = new Object("cube.obj");
Object* teapot = new Object("teapot.obj");


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("0716207 呂思函 HW1");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
	glutTimerFunc(0, timer3, 0);
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
	glDepthFunc(GL_LEQUAL);

	///TO DO: add Viewport Matrix, Projection Matrix, ModelView Matrix, lighting
	///  Viewport Matrix
	glViewport(0,0, 500, 500);
	/// Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (float)windowSize[0]/(float)windowSize[1], 1.0, 1000.0);/// fovy, aspect, near, far
	///  ModelView Matrix 
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    gluLookAt(20, 20, 20, 0, 0, 0, 0, 1, 0);/// eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz
	light();

	///TO DO: add model, translate, rotate, scale
	glTranslatef(translate_x, translate_y, translate_z);
	glRotatef(zAngle, 0, 0, 1);
	glRotatef(yAngle, 0, 1, 0);
	glRotatef(xAngle, 1, 0, 0);

	glRotatef(ang, 0, 1, 0);
	glPushMatrix();
	DrawBasis();
	glPopMatrix();

	glTranslatef(-3, 0, 3);
	glPushMatrix();
	LoadModel(teapot);
	glPopMatrix();
	
	glTranslatef(6, 0, -6);
	glRotatef(ang3, 0, 1, 0);
	glPushMatrix();
	LoadModel(elephant);
	glPopMatrix();

	glTranslatef(-2, 0, -2);
	glScalef(0.5, 0.5, 0.5);
	glPushMatrix();
	LoadModel(cube);
	glPopMatrix();

	glutPostRedisplay();
	glutSwapBuffers();
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

void reshape(GLsizei w, GLsizei h)
{
	windowSize[0] = w;
	windowSize[1] = h;
}

void DrawBasis()
{
	///TO DO: draw basis
	const double PI = 3.14159265358979323846;
	double r = 10;
	int sides = 20;

	/// top
	glBegin(GL_POLYGON);
	for( int i = 0; i < sides; i++ ) 
	{
		double angle = PI * i * 2/ sides;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3d(r * sin(angle),  0.0, r * cos(angle));
	}
	glEnd();

	/// bottom
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
	{
		double angle = PI * i * 2 / sides;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3d(r * sin(angle), -5.0, r * cos(angle));
	}
	glEnd();

	/// draw square
	glBegin(GL_QUADS);
	for (int i = 0; i < sides; i++)
	{
		double t = i;
		double angle = PI * i * 2 / sides;
		double tmp =  PI * (t+1) * 2 / sides;

		///  左下
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f( sin(angle), 0.0, cos(angle));
		glVertex3d(r * sin(angle), -5.0, r *cos(angle));
		/// 右下
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(sin(angle), 0.0, cos(angle));
		glVertex3d(r * sin(tmp), -5.0, r * cos(tmp));
		/// 右上
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(sin(angle), 0.0, cos(angle));
		glVertex3d(r * sin(tmp), 0.0, r * cos(tmp));
		///  左上
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(sin(angle), 0.0, cos(angle));
		glVertex3d(r * sin(angle), 0.0, r * cos(angle));
	}	
	glEnd();
}

void reset()
{
	xAngle = 0.0;
	yAngle = 90.0;
	zAngle = 0.0;
	translate_x = 0.0;
	translate_y = 0.0;
	translate_z = 0.0;
}

void keyboard(unsigned char key, int x, int y)
{
	/// TO DO: Set the keyboard control
	switch (key) {
		case 'x': xAngle += 5;  break;
		case 'y': yAngle += 5;  break;
		case 'z': zAngle += 5;  break;
	}
	///  zoom
	if (key == 'i')
	{
		translate_x++;
		translate_z++;
	}
	else if (key == 'o')
	{
		translate_x--;
		translate_z--;
	}
	///  reset
	else if (key == 'r')
	{
		reset();
	}
	glutPostRedisplay();
}

void timer(int t)
{
	glutTimerFunc(1, timer, 0);
	ang++;
	if (ang > 360)
		ang = ang - 360.0;
	glutPostRedisplay();
}

void timer3(int t)
{
	glutTimerFunc(3, timer3, 0);
	ang3 += 3;
	if (ang3 > 360)
		ang3 = ang3 - 360.0;
	glutPostRedisplay();
}


