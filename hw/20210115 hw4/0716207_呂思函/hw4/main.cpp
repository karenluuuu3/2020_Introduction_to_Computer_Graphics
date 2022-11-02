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

static void Timer(int v);
void shaderInit();
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void DrawUmbreon(GLuint);
void LoadTexture(unsigned int&, const char*);

GLuint Normalprogram, Umbreonprogram, Phongprogram, Explosionprogram;
GLuint VAO, VBO[3];
unsigned int modeltexture;
float windowSize[2] = { 600, 600 };
float angle = 0.0f;
int counter = 0;
int flag = 0;
float height = 0.0f;
glm::vec3 WorldLightPos = glm::vec3(2, 5, 5);
glm::vec3 WorldCamPos = glm::vec3(7.5, 5.0, 7.5);

Object* model = new Object("Umbreon.obj");

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw4");

	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

static void Timer(int value) {
	if (angle >= 6) {// 原地抖動
		angle = 0.0;
		counter++;
		if (counter == 2500)	flag = 1;// 往上飛
	}

	if (flag == 0) {//三段式抖動
		if (counter <= 1000) angle += 0.1;
		else if (counter > 1000 && counter <= 3000) angle += 1;
		else if (counter > 3000 && counter < 6000) angle += 5;
	}
	else if (flag == 1) {
		angle += 0.5;
		height += 0.0001;
		counter++;
		if (height > 6) {
			flag = -1;// explode
			height = 0;
		}
	}
	else if (flag == -1) {
		angle=0;
	}
	glutPostRedisplay();
	// 100 milliseconds
	glutTimerFunc(100, Timer, 0);
}

void shaderInit() {
	GLuint vert = createShader("Shaders/normal.vert", "vertex");
	GLuint geom = createShader("Shaders/normal.geom", "geometry");
	GLuint frag = createShader("Shaders/normal.frag", "fragment");
	Normalprogram = createProgram(vert, geom,frag);

	vert = createShader("Shaders/Umbreon.vert", "vertex");
	frag = createShader("Shaders/Umbreon.frag", "fragment");
	Umbreonprogram = createProgram(vert, 0, frag);

	vert = createShader("Shaders/Phongshading.vert", "vertex");
	frag = createShader("Shaders/Phongshading.frag", "fragment");
	Phongprogram = createProgram(vert, 0, frag);

	vert = createShader("Shaders/Explosion.vert", "vertex");
	geom = createShader("Shaders/Explosion.geom", "geometry");
	 frag = createShader("Shaders/Explosion.frag", "fragment");
	 Explosionprogram = createProgram(vert, geom, frag);
}

void bindbufferInit() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(3, VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->positions[0]) * model->positions.size(), &model->positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->normals[0]) * model->normals.size(), &model->normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model->texcoords[0]) * model->texcoords.size(), &model->texcoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void textureInit() {
	LoadTexture(modeltexture, "Umbreon.jpg");
}

glm::mat4 getV()
{
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP()
{
	// set perspective view
	float fov = 45.0f;
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

	Timer(0);

	if (flag == -1)	DrawUmbreon(Explosionprogram);
	else{
		if (flag == 0)	DrawUmbreon(Normalprogram);		
		DrawUmbreon(Phongprogram);
	}
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

void idle() {
	glutPostRedisplay();
}

void DrawUmbreon(GLuint program)
{
	glUseProgram(program);

	glm::mat4 M(1.0f);
	if (flag == 0)	M = glm::rotate(M, glm::radians(angle), glm::vec3(0, 1, 0));
	if (flag == 1)	M = glm::translate(M, glm::vec3(0, height, 0));
	
	GLuint ModelMatrixID = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	ModelMatrixID = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	ModelMatrixID = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &P[0][0]);


	glm::vec3 time = glm::vec3(angle, angle, angle);
	ModelMatrixID = glGetUniformLocation(program, "time");
	glUniform3fv(ModelMatrixID, 1, &time[0]);

	glm::vec3 Ka = glm::vec3(1, 1, 1);// ambient reflectivity
	ModelMatrixID = glGetUniformLocation(program, "Ka");
	glUniform3fv(ModelMatrixID, 1, &Ka[0]);
	glm::vec3 Kd = glm::vec3(1, 1, 1);// diffuse reflectivity
	ModelMatrixID = glGetUniformLocation(program, "Kd");
	glUniform3fv(ModelMatrixID, 1, &Kd[0]);
	glm::vec3 Ks = glm::vec3(1, 1, 1);// specular reflectivity
	ModelMatrixID = glGetUniformLocation(program, "Ks");
	glUniform3fv(ModelMatrixID, 1, &Ks[0]);

	glm::vec3 La = glm::vec3(0.2, 0.2, 0.2);// ambient intensity
	ModelMatrixID = glGetUniformLocation(program, "La");
	glUniform3fv(ModelMatrixID, 1, &La[0]);
	glm::vec3 Ld = glm::vec3(0.8, 0.8, 0.8);// diffuse intensity
	ModelMatrixID = glGetUniformLocation(program, "Ld");
	glUniform3fv(ModelMatrixID, 1, &Ld[0]);
	glm::vec3 Ls = glm::vec3(0.5, 0.5, 0.5);// specular intensity
	ModelMatrixID = glGetUniformLocation(program, "Ls");
	glUniform3fv(ModelMatrixID, 1, &Ls[0]);
	glm::vec3 gloss = glm::vec3(25.0, 25.0, 25.0);// specular shininess factor
	ModelMatrixID = glGetUniformLocation(program, "gloss");
	glUniform3fv(ModelMatrixID, 1, &gloss[0]);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, modeltexture);
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0,  3*model->fNum);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}


