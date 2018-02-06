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
#define far 10000.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5

GLfloat a = 0.0f;
GLfloat projectionMatrix[] = {
	    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
      0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
      0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
			0.0f, 0.0f, -1.0f, 0.0f };

GLuint program;

Model *bladeModel;
Model *bunnyModel;
Model *balconyModel;
Model *roofModel;
Model *wallsModel;
Model *groundModel;
Model *cowModel;
Model *skyModel;
GLfloat t = 0.0f;

GLuint groundTex;
GLuint skyTex;

vec3 camPos = {10.0f, 0.0f, 0.0f};
vec3 camLookAt = {0.0f, 0.0f, 0.0f};
vec3 camUp = {0.0f, 1.0f, 0.0f};
mat4 worldToView;

Point3D lightSourcesColorsArr[] = { {1.0f, 0.0f, 0.0f}, // Red light
                                 {0.0f, 1.0f, 0.0f}, // Green light
                                 {0.0f, 0.0f, 1.0f}, // Blue light
                                 {1.0f, 1.0f, 1.0f} }; // White light
GLfloat specularExponent[] = {10.0, 20.0, 60.0, 5.0};
GLint isDirectional[] = {0,0,1,1};
Point3D lightSourcesDirectionsPositions[] = { {10.0f, 5.0f, 0.0f}, // Red light, positional
                                       {0.0f, 5.0f, 10.0f}, // Green light, positional
                                       {-1.0f, 0.0f, 0.0f}, // Blue light along X
                                       {0.0f, 0.0f, -1.0f} }; // White light along Z

void handleKeyboardEvent();
void drawSkybox();
void drawGround();
void drawWindmill(mat4 transCompleteModel);
void draw(Model* model, mat4 mdlMatrix);



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
	bunnyModel = LoadModelPlus("./stanford-bunny.obj");
	groundModel = LoadModelPlus("./ground.obj");
	cowModel = LoadModelPlus("./cow.obj");
	skyModel = LoadModelPlus("./skybox.obj");

	LoadTGATextureSimple("grass.tga", &groundTex);
	LoadTGATextureSimple("SkyBox512.tga", &skyTex);

	// Orthogonalize and normalize up vector for camera
	vec3 planarComp = CrossProduct(VectorSub(camLookAt, camPos), camUp);
	camUp = Normalize(CrossProduct(planarComp, VectorSub(camLookAt, camPos)));

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
	program = loadShaders("lab3-4.vert", "lab3-4.frag");
	printError("init shader");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, groundTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, skyTex);

	glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_TRUE, projectionMatrix);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, (GLfloat*) &worldToView);

	glUniform3fv(glGetUniformLocation(program, "lightSourcesDirPosArr"), 4, &lightSourcesDirectionsPositions[0].x);
	glUniform3fv(glGetUniformLocation(program, "lightSourcesColorArr"), 4, &lightSourcesColorsArr[0].x);
	glUniform1fv(glGetUniformLocation(program, "specularExponent"), 4, specularExponent);
	glUniform1iv(glGetUniformLocation(program, "isDirectional"), 4, isDirectional);



	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	handleKeyboardEvent();

	// World to view
	worldToView = lookAtv(camPos, camLookAt, camUp);
	glUniformMatrix4fv(glGetUniformLocation(program, "worldToViewMatrix"), 1, GL_TRUE, (GLfloat*) &worldToView);

	glUniform3fv(glGetUniformLocation(program, "cameraPos"), 1, &camPos.x);


	// Time
	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "elapsedTime"), t);


	drawSkybox();
	drawGround();
	drawWindmill(T(0,0,0));


	glUniform1i(glGetUniformLocation(program, "use_tex"), 0);
	draw(bunnyModel, Mult(Mult(T(55,0,14), S(10,10,10)),Ry(a)));
	draw(cowModel, Mult(T(25,2,-30),S(3,3,3)));
	draw(cowModel, Mult(T(15,2,50),S(3,3,3)));
	draw(cowModel, Mult(T(20,2,5*sin(a)),S(3,3,3)));

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


void draw(Model* model, mat4 mdlMatrix)
{
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, mdlMatrix.m);
	DrawModel(model, program, "in_Position", "in_Normal", "inTexCoord");
}


void drawGround()
{
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, T(0,0,0).m);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 0);
	glUniform1i(glGetUniformLocation(program, "use_tex"), 1);
	DrawModel(groundModel, program,"in_Position","in_Normal","inTexCoord");
}


void drawSkybox()
{
	glDisable(GL_DEPTH_TEST);
	glUniform1i(glGetUniformLocation(program, "use_tex"), 1);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, T(camPos.x,camPos.y - 0.2,camPos.z).m);
	glUniform1i(glGetUniformLocation(program, "texUnit"), 1); // Texture unit 0
	DrawModel(skyModel, program,"in_Position","in_Normal","inTexCoord");

	glEnable(GL_DEPTH_TEST);
}


void handleKeyboardEvent()
{
	if (glutKeyIsDown('w'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorAdd(camLookAt, stepDirection);
		camPos = VectorAdd(camPos, stepDirection);
	}

	if (glutKeyIsDown('s'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorSub(camLookAt, stepDirection);
		camPos = VectorSub(camPos, stepDirection);
	}

	if (glutKeyIsDown('d'))
	{
		vec3 stepDirection = CrossProduct(VectorSub(camLookAt, camPos), camUp);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorAdd(camLookAt, stepDirection);
		camPos = VectorAdd(camPos, stepDirection);
	}

	if (glutKeyIsDown('a'))
	{
		vec3 stepDirection = CrossProduct(VectorSub(camLookAt, camPos), camUp);
		stepDirection = Normalize(stepDirection);
		camLookAt = VectorSub(camLookAt, stepDirection);
		camPos = VectorSub(camPos, stepDirection);
	}

	if (glutKeyIsDown('q'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(camUp, 0.05)), stepDirection);
				camLookAt = VectorAdd(camPos, camLookAt);
	}

	if (glutKeyIsDown('e'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(camUp, -0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);
	}

	if (glutKeyIsDown('r'))
	{
		camLookAt = VectorAdd(camLookAt, Normalize(camUp));
		camPos = VectorAdd(camPos, Normalize(camUp));
	}

	if (glutKeyIsDown('f'))
	{
		camLookAt = VectorSub(camLookAt, Normalize(camUp));
		camPos = VectorSub(camPos, Normalize(camUp));
	}

	if (glutKeyIsDown('t'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		vec3 planarComp = CrossProduct(stepDirection, camUp);

		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(planarComp, 0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);

		camUp = Normalize(CrossProduct(planarComp, VectorSub(camLookAt, camPos)));
	}

	if (glutKeyIsDown('g'))
	{
		vec3 stepDirection = VectorSub(camLookAt, camPos);
		vec3 planarComp = CrossProduct(stepDirection, camUp);

		camLookAt = MultMat3Vec3(mat4tomat3(ArbRotate(planarComp, -0.05)), stepDirection);
		camLookAt = VectorAdd(camPos, camLookAt);

		camUp = Normalize(CrossProduct(planarComp, VectorSub(camLookAt, camPos)));
	}
}


void drawWindmill(mat4 transCompleteModel)
{
	GLfloat roofColor[] = {153.0f/255.0f, 76.0f/255.0f, 0.0f};
	GLfloat balconyColor[] = {102.0f/255.0f, 51.0f/255.0f, 0.0f};
	GLfloat wallsColor[] = {0.5f, 0.5f, 0.5f};
	GLfloat bladeColor[] = {205.0f/255.0f, 133.0f/255.0f, 63.0f/255.0f};


	mat4 transBlade;
	mat4 rotBlade1, rotBlade2, rotBlade3, rotBlade4;
	mat4 totalBlade1, totalBlade2, totalBlade3, totalBlade4;
	mat4 rotBalcony, transBalcony, totalBalcony;
	mat4 rotRoof, transRoof, totalRoof;
	mat4 rotWalls, transWalls, totalWalls;


	glUniform1i(glGetUniformLocation(program, "use_tex"), 0);


	// Blade
	a += 0.02f;
	mat4 rotBlade = Rx(a);

	rotBlade1 = Mult(Rx(0),rotBlade);
	rotBlade2 = Mult(Rx(M_PI/2),rotBlade);
	rotBlade3 = Mult(Rx(M_PI),rotBlade);
	rotBlade4 = Mult(Rx(3*M_PI/2),rotBlade);
	transBlade = T(4.5, 9.25, 0);

	totalBlade1 = Mult(transCompleteModel, Mult(transBlade,rotBlade1));
	totalBlade2 = Mult(transCompleteModel, Mult(transBlade,rotBlade2));
	totalBlade3 = Mult(transCompleteModel, Mult(transBlade,rotBlade3));
	totalBlade4 = Mult(transCompleteModel, Mult(transBlade,rotBlade4));

	glUniform3f(glGetUniformLocation(program, "color"), bladeColor[0], bladeColor[1], bladeColor[2]);

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade1.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade2.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade3.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");

	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBlade4.m);
	DrawModel(bladeModel, program,"in_Position","in_Normal","inTexCoord");


	// Balcony
	rotBalcony = Ry(0);
	transBalcony = T(0, 0, 0);
	totalBalcony = Mult(transCompleteModel, Mult(transBalcony,rotBalcony));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalBalcony.m);
	glUniform3f(glGetUniformLocation(program, "color"), balconyColor[0], balconyColor[1], balconyColor[2]);
	DrawModel(balconyModel, program,"in_Position","in_Normal","inTexCoord");

	// Roof
	rotRoof = Mult(Rx(0),Ry(0));
	transRoof = T(0, 0, 0);
	totalRoof = Mult(transCompleteModel, Mult(transRoof,rotRoof));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalRoof.m);
	glUniform3f(glGetUniformLocation(program, "color"), roofColor[0], roofColor[1], roofColor[2]);
	DrawModel(roofModel, program,"in_Position","in_Normal","inTexCoord");

	// Walls
	rotWalls = Ry(0);
	transWalls = T(0, 0, 0);
	totalWalls = Mult(transCompleteModel, Mult(transWalls,rotWalls));
	glUniformMatrix4fv(glGetUniformLocation(program, "mdlMatrix"), 1, GL_TRUE, totalWalls.m);
	glUniform3f(glGetUniformLocation(program, "color"), wallsColor[0], wallsColor[1], wallsColor[2]);
	DrawModel(wallsModel, program,"in_Position","in_Normal","inTexCoord");
}
