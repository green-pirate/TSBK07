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
#include "loadobj.h"
#include <math.h>
#include <stdio.h>

// Globals
// Data would normally be read from files
GLfloat vertices[] =
{
	-0.5f,-0.5f,-0.5f, // triangle 1 : begin
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
    0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
    0.5f,-0.5f, 0.5f
};

GLfloat a = 0.0f;

GLfloat myMatrix[] = {1.0f, 0.0f, 0.0f, 0.0f,
	  									0.0f, 1.0f, 0.0f, 0.0f,
  										0.0f, 0.0f, 1.0f, 0.0f,
  										0.0f, 0.0f, 0.0f, 1.0f };

GLuint program;

Model *m;
GLfloat t = 0.0f;

// vertex array object
unsigned int vertexArrayObjID;
unsigned int bunnyTexCoordBufferObjID;
unsigned int bunnyVertexArrayObjID;
unsigned int bunnyVertexBufferObjID;
unsigned int bunnyIndexBufferObjID;
unsigned int bunnyNormalBufferObjID;

void OnTimer(int value)
{
    glutPostRedisplay();
    glutTimerFunc(20, &OnTimer, value);
}

void init(void)
{
	m = LoadModel("bunnyplus.obj");

	// vertex buffer object, used for uploading the geometry
	// Reference to shader program

	dumpInfo();

	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glEnable(GL_DEPTH_TEST);
	printError("GL inits");

	// Load and compile shader
	program = loadShaders("lab2-1.vert", "lab2-1.frag");
	printError("init shader");

	// Upload geometry to the GPU:

	// Allocate and activate Vertex Array Object
	glGenVertexArrays(1, &bunnyVertexArrayObjID);
	glGenBuffers(1, &bunnyTexCoordBufferObjID);
  glGenBuffers(1, &bunnyVertexBufferObjID);
  glGenBuffers(1, &bunnyIndexBufferObjID);
  glGenBuffers(1, &bunnyNormalBufferObjID);

  glBindVertexArray(bunnyVertexArrayObjID);

	printError("Start");

	// VBO for vertex data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyVertexBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Position"));
	printError("in_Position");
  // VBO for normal data
  glBindBuffer(GL_ARRAY_BUFFER, bunnyNormalBufferObjID);
  glBufferData(GL_ARRAY_BUFFER, m->numVertices*3*sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
  glVertexAttribPointer(glGetAttribLocation(program, "in_Normal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(glGetAttribLocation(program, "in_Normal"));
	printError("in_Normal");
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyIndexBufferObjID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m->numIndices*sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);
printError("skickar_inde");
	if (m->texCoordArray != NULL)
	{
	    glBindBuffer(GL_ARRAY_BUFFER, bunnyTexCoordBufferObjID);
	    glBufferData(GL_ARRAY_BUFFER, m->numVertices*2*sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
	    glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
	}
printError("inTexCoord");
	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);
printError("myMa");
	glUniform1f(glGetUniformLocation(program, "elapsedTime"), t);
printError("Et");
	// End of upload of geometry

	printError("init arrays");
}


void display(void)
{
	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(bunnyVertexArrayObjID);    // Select VAO
  glDrawElements(GL_TRIANGLES, m->numIndices, GL_UNSIGNED_INT, 0L);

	//glBindVertexArray(vertexArrayObjID);	// Select VAO
	//glDrawArrays(GL_TRIANGLES, 0, 36);	// draw object

	printError("display");

	glutSwapBuffers();

	myMatrix[0] = cos(a)*cos(a);
	myMatrix[1] = -cos(a)*sin(a);
	myMatrix[2] = sin(a);
	myMatrix[4] = sin(a)*sin(a)*cos(a) + cos(a)*sin(a);
	myMatrix[5] = -sin(a)*sin(a)*sin(a) + cos(a)*cos(a);
	myMatrix[6] = -sin(a)*cos(a);
	myMatrix[8] = -cos(a)*sin(a)*cos(a) + sin(a)*sin(a);
	myMatrix[9] = cos(a)*sin(a)*sin(a) + sin(a)*cos(a);
	myMatrix[10] = cos(a)*cos(a);
	a += 0.05f;

	glUniformMatrix4fv(glGetUniformLocation(program, "myMatrix"), 1, GL_TRUE, myMatrix);

	t = (GLfloat)glutGet(GLUT_ELAPSED_TIME);
	glUniform1f(glGetUniformLocation(program, "elapsedTime"), t);

}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutCreateWindow ("GL3 white triangle example");
	glutDisplayFunc(display);
	init ();
	glutTimerFunc(20, &OnTimer, 0);
	glutMainLoop();
	return 0;
}
