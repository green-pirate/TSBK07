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
#define far 60.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat a = 0.0f;
GLfloat b = 0.0f;
GLfloat c = 0.0f;
GLfloat d = 0.0f;
GLfloat projectionMatrix[] = {
	    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
      0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
      0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
			0.0f, 0.0f, -1.0f, 0.0f };

GLuint program;

Model *bladeModel;
Model *balconyModel;
Model *roofModel;
Model *wallsModel;
GLfloat t = 0.0f;
mat4 transBlade;
mat4 rotBlade1, rotBlade2, rotBlade3, rotBlade4;
mat4 totalBlade1, totalBlade2, totalBlade3, totalBlade4;
mat4 rotBalcony, transBalcony, totalBalcony;
mat4 rotRoof, transRoof, totalRoof;
mat4 rotWalls, transWalls, totalWalls;
GLuint myTex;
GLuint anotherTex;

vec3 camPos = {30.0f, 10.0f, 0.0f};
vec3 camLookAt = {0.0f, 5.0f, 0.0f};
vec3 camUp = {0.0f, 1.0f, 0.0f};
mat4 worldToView;


void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	bladeModel = LoadModelPlus("./windmill/blade.obj");
	balconyModel = LoadModelPlus("./windmill/windmill-balcony.obj");
	roofModel = LoadModelPlus("./windmill/windmill-roof.obj");
	wallsModel = LoadModelPlus("./windmill/windmill-walls.obj");
	//LoadTGATextureSimple("rutor.tga", &myTex);
	//LoadTGATextureSimple("maskros512.tga", &anotherTex);

	worldToView = lookAtv(camPos, camLookAt, camUp);

	// vertex buffer object, used for uploading the geometry
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0,0.2,0,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab3-1.vert", "lab3-1.frag");
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
	//camPosNew.x = sqrt(camPos.x*camPos.x + camPos.y*camPos.y + camPos.z*camPos.z)* cos(a);
	//camPosNew.z = sqrt(camPos.x*camPos.x + camPos.y*camPos.y + camPos.z*camPos.z)* sin(a);
	//worldToView = lookAtv(camPosNew, camLookAt, camUp);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, (GLfloat*) &worldToView);
	// Time
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "elapsedTime"), t);

	// Blade
	a += 0.02f;
	mat4 rotBlade = Rx(a);
	
	rotBlade1 = Mult(Rx(0),rotBlade);
	rotBlade2 = Mult(Rx(M_PI/2),rotBlade);
	rotBlade3 = Mult(Rx(M_PI),rotBlade);
	rotBlade4 = Mult(Rx(3*M_PI/2),rotBlade);
	transBlade = T(4.5, 9.25, 0);

	mat4 transCompleteModel = T(0, 0, 5*sin(4*a));

	totalBlade1 = Mult(transCompleteModel, Mult(transBlade,rotBlade1));
	totalBlade2 = Mult(transCompleteModel, Mult(transBlade,rotBlade2));
	totalBlade3 = Mult(transCompleteModel, Mult(transBlade,rotBlade3));
	totalBlade4 = Mult(transCompleteModel, Mult(transBlade,rotBlade4));

	GLfloat roofColor[] = {153.0f/255.0f, 76.0f/255.0f, 0.0f};
	GLfloat balconyColor[] = {102.0f/255.0f, 51.0f/255.0f, 0.0f};
	GLfloat wallsColor[] = {0.5f, 0.5f, 0.5f};
	GLfloat bladeColor[] = {205.0f/255.0f, 133.0f/255.0f, 63.0f/255.0f};

	glUniform3f(glGetUniformLocation(program, "color"), bladeColor[0], bladeColor[1], bladeColor[2]);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade1.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade2.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade3.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade4.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");


	// Balcony
	b += 0.05f;
	rotBalcony = Ry(0);
	transBalcony = T(0, 0, 0);
	//transCar = T(0, 0, -10 + 8*sin(0.001*t));
	//rot = Rx();
	totalBalcony = Mult(transCompleteModel, Mult(transBalcony,rotBalcony));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBalcony.m);
	glUniform3f(glGetUniformLocation(program, "color"), balconyColor[0], balconyColor[1], balconyColor[2]);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 1
	DrawModel(balconyModel, program,"in_Position","in_Normal","inTexCoord");

	// Roof
	c += 0.05f;
	rotRoof = Mult(Rx(0),Ry(0));
	transRoof = T(0, 0, 0);
	//trans = T(0, 0, -10 + 8*sin(0.001*t));
	//rot = Rx();
	totalRoof = Mult(transCompleteModel, Mult(transRoof,rotRoof));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalRoof.m);
	glUniform3f(glGetUniformLocation(program, "color"), roofColor[0], roofColor[1], roofColor[2]);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
	DrawModel(roofModel, program,"in_Position","in_Normal","inTexCoord");

	// Walls
	d += 0.05f;
	rotWalls = Ry(0);
	transWalls = T(0, 0, 0);
	//transCar = T(0, 0, -10 + 8*sin(0.001*t));
	//rot = Rx();
	totalWalls = Mult(transCompleteModel, Mult(transWalls,rotWalls));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalWalls.m);
	glUniform3f(glGetUniformLocation(program, "color"), wallsColor[0], wallsColor[1], wallsColor[2]);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 1
	DrawModel(wallsModel, program,"in_Position","in_Normal","inTexCoord");

	printError("display");

	glutSwapBuffers();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("Lab3-1");
	glutDisplayFunc(display);
	init();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}