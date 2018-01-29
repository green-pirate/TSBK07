// Lab 1-1.
// This is the same as the first simple example in the course book,
// but with a few error checks.
// Remember to copy your file to a new on appropriate places during the lab so you keep old results.
// Note that the files "lab1-1.frag", "lab1-1.vert" are required.

// Should work as is on Linux and Mac. MS Windows needs GLEW or glee.
// See separate Visual Studio version of my demos.
#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	// Linking hint for Lightweight IDE
	// uses framework Cocoa
#endif
#include "MicroGlut.h"
#include "GL_utilities.h"
#include "LoadTGA.h"
#include "loadobj.h"
#include "VectorUtils3.h"
#include <math.h>
#include <stdio.h>

// Globals
#define near 1.0
#define far 30.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat a = 90.0f;
GLfloat b = 90.0f;
GLfloat projectionMatrix[] = {
	    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
      0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
      0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
			0.0f, 0.0f, -1.0f, 0.0f };

GLuint program;

Model *bunnyModel;
Model *carModel;
GLfloat t = 0.0f;
mat4 rotBunny, transBunny, totalBunny;
mat4 rotCar, transCar, totalCar;
GLuint myTex;
GLuint anotherTex;

vec3 camPos = {5.0f, 5.0f, 5.0f};
vec3 camLookAt = {0.0f, 0.0f, 0.0f};
vec3 camUp = {0.0f, 1.0f, 0.0f};
mat4 worldToView;


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	bunnyModel = LoadModelPlus("bunnyplus.obj");
	carModel = LoadModelPlus("bilskiss.obj");
	LoadTGATextureSimple("rutor.tga", &myTex);
	LoadTGATextureSimple("maskros512.tga", &anotherTex);

	worldToView = lookAtv(camPos, camLookAt, camUp);

	// vertex buffer object, used for uploading the geometry
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-7.vert", "lab2-7.frag");
	printError("init shader");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, anotherTex);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, (GLfloat*) &worldToView);

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// World to view
	vec3 camPosNew = camPos;
	camPosNew.x = sqrt(camPos.x*camPos.x + camPos.y*camPos.y + camPos.z*camPos.z)* cos(a);
	camPosNew.z = sqrt(camPos.x*camPos.x + camPos.y*camPos.y + camPos.z*camPos.z)* sin(a);
	worldToView = lookAtv(camPosNew, camLookAt, camUp);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, (GLfloat*) &worldToView);
	// Time
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "elapsedTime"), t);

	// Bunny
	a += 0.05f;
	rotBunny = Mult(Rx(0),Ry(0));
	transBunny = T(0, 0, 0);
	//trans = T(0, 0, -10 + 8*sin(0.001*t));
	//rot = Rx();
	totalBunny = Mult(transBunny,rotBunny);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBunny.m);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	DrawModel(bunnyModel, program,"in_Position","in_Normal","inTexCoord");

	// Car
	b += 0.05f;
	rotCar = Ry(0);
	transCar = T(0, 0, -4);
	//transCar = T(0, 0, -10 + 8*sin(0.001*t));
	//rot = Rx();
	totalCar = Mult(transCar,rotCar);
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalCar.m);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 1
	DrawModel(carModel, program,"in_Position","in_Normal","inTexCoord");

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
