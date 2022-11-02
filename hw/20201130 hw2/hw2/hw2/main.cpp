#define GLM_ENABLE_EXPERIMENTAL

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>

using namespace std;

void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void keyboard(unsigned char key, int x, int y);
void DrawBasis();
void DrawUmbreon();
void LoadTexture(unsigned int&, const char*);


GLuint program;
GLuint VAO, VBO[3];// VBO[3]���}�spositions, normals, texcoords(�T��buffer)
unsigned int basistexture, modeltexture;
int windowSize[2] = { 600, 600 };
float angle = 0.0f;

Object* model = new Object("UmbreonHighPoly.obj");


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw2");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void shaderInit() {
	//// TODO: ////
	//
	// Hint:
	// 1. createShader
	// 2. createProgram
	GLuint vert = createShader("Shaders/vertexShader.vert", "vertex");
	GLuint frag = createShader("Shaders/fragmentShader.frag", "fragment");
	program = createProgram(vert, frag);
}

void bindbufferInit() {
	//// TODO: ////
	//	 Hint:
	// 1. Setup VAO
	// 2. Setup VBO of vertex positions, normals, and texcoords

	// generate a new array object
	glGenVertexArrays(1, &VAO);
	// bind a vertex array object
	glBindVertexArray(VAO);

	// vertex
	glGenBuffers(1, &VBO[0]);//generate a new buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);// how to use buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size(model->positions), model->positions.data(), GL_STATIC_DRAW);// copy vertex data to the buffer object
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// stride : xyz�F�_�l�I�q0�}�l
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);// unbind VBO
	glBindVertexArray(0);// unbind VAO

	// normal
	glGenVertexArrays(1, &VBO[1]);//generate a new buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);// how to use buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size(model->normals), model->normals.data(), GL_STATIC_DRAW);// copy vertex data to the buffer object
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);// stride : nx, ny, nz�F�_�l�I�q0�}�l
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);// unbind VBO
	glBindVertexArray(1);// unbind VAO

	// texcoord
	glGenVertexArrays(1, &VBO[2]);//generate a new buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);// how to use buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size(model->texcoords), model->texcoords.data(), GL_STATIC_DRAW);// copy vertex data to the buffer object
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);// stride : s0, t0�F�_�l�I�q0�}�l
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);// unbind VBO
	glBindVertexArray(2);// unbind VAO
}

void textureInit() {
	LoadTexture(basistexture, "basis.jpg");
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(7.5, 5.0, 7.5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 60.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void display() {
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	DrawUmbreon();
	DrawBasis();
	angle += 0.1;
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

void keyboard(unsigned char key, int x, int y) {

}

void idle() {
	glutPostRedisplay();
}

void DrawBasis() {
	// viewport transformation
	glViewport(0, 0, windowSize[0], windowSize[1]);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)windowSize[0] / (GLfloat)windowSize[1], 1.0, 1000.0);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(7.5, 5.0, 7.5,// eye
		0.0, 0.0, 0.0,     // center
		0.0, 1.0, 0.0);    // up

	glPushMatrix();
	glRotatef(angle, 0, 1, 0);

	//// TODO: ////
	//
	// draw the basis and make the side of basis with texture
	const double PI = 3.14159265358979323846;
	double r = 4;
	int sides = 20;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, basistexture);

	/// top
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
	{
		double ang = PI * i * 2 / sides;
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3d(r * sin(ang), 0.0, r * cos(ang));
	}
	glEnd();

	/// bottom
	glBegin(GL_POLYGON);
	for (int i = 0; i < sides; i++)
	{
		double ang = PI * i * 2 / sides;
		//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3d(r * sin(ang), -3.0, r * cos(ang));
	}
	glEnd();

	/// draw square
	glBegin(GL_QUADS);
	for (int i = 0; i < sides; i++)
	{
		double t = i;
		double ang = PI * i * 2 / sides;
		double tmp = PI * (t + 1) * 2 / sides;

		///  ���U
		glNormal3f(sin(ang), 0.0, cos(ang));
		glTexCoord2f(1.0f, 0.0f);
		glVertex3d(r * sin(ang), -3.0, r * cos(ang));
		/// �k�U
		glNormal3f(sin(ang), 0.0, cos(ang));
		glTexCoord2f(1.0f, 1.0f);
		glVertex3d(r * sin(tmp), -3.0, r * cos(tmp));
		/// �k�W
		glNormal3f(sin(ang), 0.0, cos(ang));
		glTexCoord2f(0.0f, 1.0f);
		glVertex3d(r * sin(tmp), 0.0, r * cos(tmp));
		///  ���W
		glNormal3f(sin(ang), 0.0, cos(ang));
		glTexCoord2f(0.0f, 0.0f);
		glVertex3d(r * sin(ang), 0.0, r * cos(ang));
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);// unbind texture
	glActiveTexture(0);

	glPopMatrix();
}

void DrawUmbreon()
{
	glUseProgram(program);// ��ܭn�έ���shader

	glm::mat4 M(1.0f);
	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	M = glm::translate(M, glm::vec3(0, 1.3, 0));

	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);// �qM[0][0]�}�l��A�e��shader

	//// TODO: ////
	// pass projection matrix, and view matrix and trigger by Uniform (use getP() amd getV())
	// also pass modeltexture to shader and trigger by Uniform

	// GLfloat pmtx[16];
	// GLfloat mmtx[16];
	glm::mat4 pmtx = getP();
	glm::mat4 mmtx = getV();

	// glGetFloatv(GL_PROJECTION_MATRIX, pmtx);
	// glGetFloatv(GL_MODELVIEW_MATRIX, mmtx);
	GLint pmatLoc = glGetUniformLocation(program, "Projection");
	GLint mmatLoc = glGetUniformLocation(program, "ModelView");

	//input the modelview matrix into vertex shader
	glUniformMatrix4fv(pmatLoc, 1, GL_FALSE, &pmtx[0][0]);// �qpmtx[0][0]�}�l��A�e��shader�A�ĤG�ӼƦr�Olocation ID
	//input the rotation matrix into vertex shader
	glUniformMatrix4fv(mmatLoc, 1, GL_FALSE, &mmtx[0][0]);//�qmmtx[0][0]�}�l��A�e��shader�A�ĤG�ӼƦr�Olocation ID

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	GLint texLoc = glGetUniformLocation(program, "Texture");
	glUniform1i(texLoc, 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_QUADS, 0, 4 * model->fNum);// draw object : 0 : starting index in the enabled arrays�F4 * model->fNum�ӳ��I�A���I����
	glBindVertexArray(0);// unbind VAO
	glBindTexture(GL_TEXTURE_2D, 0);// unbind texture
	glActiveTexture(0);
	glUseProgram(0);// switch program
}


